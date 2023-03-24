#include "grammar.h"
#include <regex.h>


Tree *treeConstruct(
        TreeType type,
        Tree *left,
        Tree *right
) {
        Tree *tree = (Tree *) calloc(1, sizeof(Tree));

        tree->type = type;
        tree->left = left;
        tree->right = right;

        return tree;
}


void treeDeconstruct(Tree *tree) {

        if (tree) {
                if (tree->left) treeDeconstruct(tree->left);
                if (tree->right) treeDeconstruct(tree->right);

                free(tree);
        }
}

Tree *parseTree(Tree *tree, Tree *parse) {

        if (tree->type == TreeTypeProposition) return parseProposition(tree, parse);
        if (tree->type == TreeTypeConjunction) return parseConjunction(tree, parse);
        if (tree->type == TreeTypeDisjunction) return parseDisjunction(tree, parse);
        if (tree->type == TreeTypeImplication) return parseImplication(tree, parse);

        return NULL;
}


//@chatgpt
Tree *parseProposition(Tree *tree, Tree *parse) {
        // parse is a tree containing the unconsumed string in `left` and optional
        // data in `right`.


        // If we're at the end of the string.

        if (! *((char *) parse->left)) {
                return NULL;
        }

        regex_t regex;
        int reti;

        // Compile regular expression
        reti = regcomp(&regex, (char *)tree->left, REG_EXTENDED);
        if (reti) {
                //fprintf(stderr, "Could not compile regex\n");
                return NULL;
        }

        // Execute regular expression
        regmatch_t match;
        reti = regexec(&regex, (char *)parse->left, 1, &match, 0);
        if (!reti && match.rm_so == 0) {
                // Match found
                size_t matched_len = match.rm_eo - match.rm_so;

                // Construct new parse tree
                char *matched_str = strndup((char *)parse->left, matched_len);
                Tree *new_parse = Parse( (((char *) parse->left) + matched_len), matched_str);

                // Free memory used by regex
                regfree(&regex);

                return new_parse;
        } else {
                // No match found
                regfree(&regex);
                return NULL;
        }
}


//@chatgpt
Tree *parseDisjunction(Tree *tree, Tree *parse) {


    // Attempt to parse the left subtree
    Tree *left = parseTree(tree->left, parse);
    if (left != NULL) {
        // Left subtree parsed successfully

        return left;
    }

    // Attempt to parse the right subtree
    Tree *right = parseTree(tree->right, parse);
    if (right != NULL) {
        // Right subtree parsed successfully
        return right;
    }

    // Neither subtree parsed successfully
    return NULL;
}


//@chatgpt
Tree *parseConjunction(Tree *tree, Tree *parse) {
    // Attempt to parse the left subtree
    Tree *left = parseTree(tree->left, parse);
    if (left == NULL) {
        // Left subtree failed to parse
        return NULL;
    }

    // Attempt to parse the right subtree
    Tree *right = parseTree(tree->right, left);
    if (right == NULL) {
        // Right subtree failed to parse
        //treeDeconstruct(left);
        return NULL;
    }

    // Both subtrees parsed successfully
    return Parse(right->left, And(left->right, right->right));
}


//@chatgpt
Tree *parseImplication(Tree *tree, Tree *parse) {
    // Evaluate the parse of the right subtree
    Tree *right = parseTree(tree->right, parse);

    if (right == NULL) {
        // Right subtree failed to parse
        return NULL;
    }

    // Apply the function to the parsed data

    void *(*func)(void *) = (void *(*)(void *)) tree->left;
    void *data = (void *) right->right;
    void *result = func(data);

    // Construct a new parse tree with the output of the function

    right->right = result;
    return right;
}
