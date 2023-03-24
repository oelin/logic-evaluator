#pragma once

#include <stdlib.h>
#include <string.h>

#define Match(l) treeConstruct( \
        TreeTypeProposition, (Tree *) (l), NULL)

#define And(l, r) treeConstruct( \
        TreeTypeConjunction, l, r)

#define Or(l, r) treeConstruct( \
        TreeTypeDisjunction, l, r)

#define Map(l, r) treeConstruct( \
        TreeTypeImplication, l, r)

#define Parse(s, x) treeConstruct( \
        TreeTypeProposition, (Tree *) (s), (Tree *) (x))


#define Variable(name) Tree *(name) = (Tree *) calloc(1, sizeof(Tree));

#define Production(name, x) memcpy(name, x, sizeof(Tree));


typedef enum {
        TreeTypeProposition,
        TreeTypeConjunction,
        TreeTypeDisjunction,
        TreeTypeImplication
} TreeType;


typedef struct Tree Tree;

struct Tree {
        Tree *left;
        Tree *right;
        TreeType type;
};


Tree *treeConstruct(TreeType, Tree *, Tree *);
void treeDeconstruct(Tree *);


Tree *parseTree(Tree *, Tree *);
Tree *parseProposition(Tree *, Tree *);
Tree *parseDisjunction(Tree *, Tree *);
Tree *parseConjunction(Tree *, Tree *);
Tree *parseImplication(Tree *, Tree *);
