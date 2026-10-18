#ifndef MSL_TREE_H
#define MSL_TREE_H

// Partial reconstruction of the MSL C++ red-black tree behind std::map and
// std::multimap (the DOL has its "tree::insert length error" message). Based on
// include/PowerPC_EABI_Support/MSL_C++/MSL_Common/msl_tree.h of smstrikers-decomp
// (see notes/PROVENANCE.md), adapted to the code of this DOL. Only what the game
// units use is declared.

#include "types.h"
#include <stdio.h>
#include <stdlib.h>

inline void* operator new(size_t, void* p) { return p; }

// stderr is &__files[2] (FILE is 0x50 bytes, see libs/MSL_C/include/file_struct.h,
// which this header does not include).
struct __msl_file_storage {
    u8 unk0[0x50];
};
extern "C" __msl_file_storage __files[];
#define __msl_stderr ((FILE*)&__files[2])

// The first error message of insert_node_at reaches fprintf through an argument
// (the order of its address computations differs from the second message).
inline void __msl_error(const char* msg) {
    fprintf(__msl_stderr, msg);
    abort();
}

namespace std {

template <class T>
class allocator {
public:
    allocator() {}
    void destroy(T* p) { p->~T(); }
    void deallocate(T* p, unsigned long) { ::operator delete(p); }
};

template <class T1, class T2>
struct pair {
    pair(const T1& f, const T2& s) : first(f), second(s) {}
    template <class U1, class U2>
    pair(const pair<U1, U2>& p) : first(p.first), second(p.second) {}

    T1 first;
    T2 second;
};

template <class T1, class T2>
inline pair<T1, T2> make_pair(T1 x, T2 y) {
    return pair<T1, T2>(x, y);
}

template <int N>
class __red_black_tree {
public:
    struct anchor {
        anchor() : left_(0) {}

        void* left_;
    };

    struct node_base : public anchor {
        node_base* parent() const { return (node_base*)((unsigned long)parent_ & ~1); }
        node_base* grandparent() const { return parent()->parent(); }
        bool is_red() const { return ((unsigned long)parent_ & 1) == 1; }
        void set_red() { parent_ = (void*)((unsigned long)parent_ | 1); }
        void set_black() { parent_ = (void*)((unsigned long)parent_ & ~1); }

        void* right_;
        void* parent_;
    };

    static void rotate_right(node_base* x, node_base*& root);
    static void rotate_left(node_base* x, node_base*& root);
    static void balance_insert(node_base* x, node_base* root);
};

template <int N>
inline void __red_black_tree<N>::rotate_left(node_base* x, node_base*& root) {
    node_base* y = (node_base*)x->right_;
    if (root == x) {
        root = y;
    }
    x->right_ = y->left_;
    node_base* yl = (node_base*)y->left_;
    if (yl != 0) {
        yl->parent_ = (void*)((unsigned long)x | ((unsigned long)yl->parent_ & 1));
    }
    y->parent_ = (void*)(((unsigned long)x->parent_ & ~1) | ((unsigned long)y->parent_ & 1));
    node_base* parent = (node_base*)((unsigned long)x->parent_ & ~1);
    if (x == (node_base*)parent->left_) {
        parent->left_ = y;
    } else {
        parent->right_ = y;
    }
    y->left_ = x;
    x->parent_ = (void*)((unsigned long)y | ((unsigned long)x->parent_ & 1));
}

template <int N>
inline void __red_black_tree<N>::rotate_right(node_base* x, node_base*& root) {
    node_base* y = (node_base*)x->left_;
    if (root == x) {
        root = y;
    }
    x->left_ = y->right_;
    node_base* yr = (node_base*)y->right_;
    if (yr != 0) {
        yr->parent_ = (void*)((unsigned long)x | ((unsigned long)yr->parent_ & 1));
    }
    y->parent_ = (void*)(((unsigned long)x->parent_ & ~1) | ((unsigned long)y->parent_ & 1));
    node_base* parent = (node_base*)((unsigned long)x->parent_ & ~1);
    if (x == (node_base*)parent->left_) {
        parent->left_ = y;
    } else {
        parent->right_ = y;
    }
    y->right_ = x;
    x->parent_ = (void*)((unsigned long)y | ((unsigned long)x->parent_ & 1));
}

template <int N>
void __red_black_tree<N>::balance_insert(node_base* x, node_base* root) {
    node_base* y;
    x->set_red();
    while (x != root && x->parent()->is_red()) {
        node_base* g = x->grandparent();
        if (x->parent() == (node_base*)g->left_) {
            y = (node_base*)g->right_;
            if (y != 0 && y->is_red()) {
                x->parent()->set_black();
                y->set_black();
                x = x->grandparent();
                x->set_red();
            } else {
                if (x == (node_base*)x->parent()->right_) {
                    x = x->parent();
                    rotate_left(x, root);
                }
                x->parent()->set_black();
                x->grandparent()->set_red();
                rotate_right(x->grandparent(), root);
            }
        } else {
            y = (node_base*)g->left_;
            if (y != 0 && y->is_red()) {
                x->parent()->set_black();
                y->set_black();
                x = x->grandparent();
                x->set_red();
            } else {
                if (x == (node_base*)x->parent()->left_) {
                    x = x->parent();
                    rotate_right(x, root);
                }
                x->parent()->set_black();
                x->grandparent()->set_red();
                rotate_left(x->grandparent(), root);
            }
        }
    }
    root->set_black();
}

} // namespace std

namespace Metrowerks {
namespace details {

template <class First, class Second, int tag>
class compressed_pair_imp {
public:
    compressed_pair_imp() : first_(), second_() {}

    First& first() { return first_; }
    Second& second() { return second_; }

private:
    First first_;
    Second second_;
};

template <class First, class Second>
class compressed_pair_imp<First, Second, 1> : private First {
public:
    compressed_pair_imp() : First(), second_() {}

    First& first() { return *this; }
    Second& second() { return second_; }

private:
    Second second_;
};

} // namespace details
} // namespace Metrowerks

namespace std {

// The node owner of the tree. In the DOL, a map destructor tests `this` five times
// before it reaches the nodes, once per class in the chain map -> __tree -> owner,
// so this owner sits three classes below __tree (the names of the intermediate
// classes are unknown). CompTag 0 keeps the comparator as a member (the comparator
// slot is then 4 bytes).
template <class T, class Compare, class Allocator, int CompTag>
class __tree_base3 : private __red_black_tree<1> {
public:
    typedef __red_black_tree<1>::node_base node_base;
    typedef __red_black_tree<1>::anchor anchor;

    struct node : public node_base {
        T data_;
    };

    __tree_base3() { comp_.second() = (node*)&node_alloc_.second(); }
    ~__tree_base3() {
        if (node_alloc_.second().left_ != 0) {
            destroy((node*)node_alloc_.second().left_);
        }
    }

    Allocator& alloc() { return alloc_.first(); }
    allocator<node>& node_alloc() { return node_alloc_.first(); }
    void destroy(node* p);

    Metrowerks::details::compressed_pair_imp<Allocator, unsigned long, 1> alloc_;
    Metrowerks::details::compressed_pair_imp<allocator<node>, anchor, 1> node_alloc_;
    Metrowerks::details::compressed_pair_imp<Compare, node*, CompTag> comp_;
};

template <class T, class Compare, class Allocator, int CompTag>
void __tree_base3<T, Compare, Allocator, CompTag>::destroy(node* p) {
    if (p->left_ != 0) {
        destroy((node*)p->left_);
    }
    if (p->right_ != 0) {
        destroy((node*)p->right_);
    }
    alloc().destroy(&p->data_);
    node_alloc().deallocate(p, 1);
}

template <class T, class Compare, class Allocator, int CompTag>
class __tree_base2 : public __tree_base3<T, Compare, Allocator, CompTag> {};

template <class T, class Compare, class Allocator, int CompTag>
class __tree_base1 : public __tree_base2<T, Compare, Allocator, CompTag> {};

// Frees a node whose element could not be built (released once it is linked).
template <class Node>
struct __tree_node_holder {
    __tree_node_holder(allocator<Node>& a, Node* p) : alloc_(&a), p_(p) {}
    ~__tree_node_holder() {
        if (p_ != 0) {
            alloc_->deallocate(p_, 1);
        }
    }
    void release() { p_ = 0; }

    allocator<Node>* alloc_;
    Node* p_;
};

template <class T, class Compare, class Allocator, int CompTag>
class __tree : public __tree_base1<T, Compare, Allocator, CompTag> {
public:
    typedef __tree_base3<T, Compare, Allocator, CompTag> base;
    typedef typename base::node node;
    typedef typename base::node_base node_base;

    class iterator {
    public:
        iterator(node* p) : ptr_(p) {}
        iterator(const iterator& other) : ptr_(other.ptr_) {}

        node* ptr_;
    };

    node* root() { return (node*)this->node_alloc_.second().left_; }
    node* end_node() { return (node*)&this->node_alloc_.second(); }

    template <class Key>
    pair<iterator, iterator> equal_range(const Key& k);

    template <class Key>
    node* find_insert_pos_multi(const Key& key, node*& prev, bool& leftchild, bool& is_leftmost);
    node* insert_node_at(node* p, bool leftchild, bool is_leftmost, const T& x);

    node* insert_multi(T x) {
        node* prev;
        bool leftchild;
        bool is_leftmost;
        node* p = find_insert_pos_multi(x.first, prev, leftchild, is_leftmost);
        return insert_node_at(p, leftchild, is_leftmost, x);
    }
};

template <class T, class Compare, class Allocator, int CompTag>
template <class Key>
typename __tree<T, Compare, Allocator, CompTag>::node*
__tree<T, Compare, Allocator, CompTag>::find_insert_pos_multi(const Key& key, node*& prev, bool& leftchild,
                                                              bool& is_leftmost) {
    prev = 0;
    node* p = end_node();
    node* n = root();
    leftchild = true;
    is_leftmost = true;
    while (n != 0) {
        p = n;
        if (key < n->data_.first) {
            n = (node*)n->left_;
            leftchild = true;
        } else {
            prev = n;
            n = (node*)n->right_;
            leftchild = false;
            is_leftmost = false;
        }
    }
    return p;
}

template <class T, class Compare, class Allocator, int CompTag>
template <class Key>
pair<typename __tree<T, Compare, Allocator, CompTag>::iterator, typename __tree<T, Compare, Allocator, CompTag>::iterator>
__tree<T, Compare, Allocator, CompTag>::equal_range(const Key& k) {
    node* x = root();
    node* y = end_node();
    while (x != 0) {
        if (k < x->data_.first) {
            y = x;
            x = (node*)x->left_;
        } else if (x->data_.first < k) {
            x = (node*)x->right_;
        } else {
            node* hi;
            node* lo;
            lo = (node*)x->left_;
            hi = (node*)x->right_;
            node* first = x;
            node* last = y;
            while (lo != 0) {
                if (!(lo->data_.first < k)) {
                    first = lo;
                    lo = (node*)lo->left_;
                } else {
                    lo = (node*)lo->right_;
                }
            }
            while (hi != 0) {
                if (k < hi->data_.first) {
                    last = hi;
                    hi = (node*)hi->left_;
                } else {
                    hi = (node*)hi->right_;
                }
            }
            return pair<iterator, iterator>(first, last);
        }
    }
    return pair<iterator, iterator>(y, y);
}

template <class T, class Compare, class Allocator, int CompTag>
typename __tree<T, Compare, Allocator, CompTag>::node*
__tree<T, Compare, Allocator, CompTag>::insert_node_at(node* p, bool leftchild, bool is_leftmost, const T& x) {
    if (this->alloc_.second() == 0xFFFFFFFFu) {
        __msl_error("tree::insert length error");
    }
    node* n = (node*)::operator new(sizeof(node));
    if (n == 0) {
        fprintf(__msl_stderr, "Memory allocation failure");
        abort();
    }
    __tree_node_holder<node> h(this->node_alloc(), n);
    // The DOL keeps the holder on the stack: it is used through a pointer.
    __tree_node_holder<node>* hp = &h;
    new (&n->data_) T(x);
    n->right_ = 0;
    hp->release();
    n->left_ = 0;
    new (&n->parent_) void*(p);
    if (leftchild) {
        p->left_ = n;
    } else {
        p->right_ = n;
    }
    ++this->alloc_.second();
    __red_black_tree<1>::balance_insert(n, root());
    if (is_leftmost) {
        this->comp_.second() = n;
    }
    return n;
}

} // namespace std

#endif // MSL_TREE_H
