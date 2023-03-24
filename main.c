#include <stdbool.h>
#include <stdio.h>

#include "grammar.h"

void *evaluateProposition(void *matchedProposition) {
  bool *value = (bool *)calloc(1, sizeof(bool));

  if (*((char *)matchedProposition) == 'T') {
    *value = true;
  } else {
    *value = false;
  }

  return value;
}

void *evaluateBinaryExpression(Tree *tree) {
  Tree *r = tree->right;
  Tree *rr = r->right;
  Tree *rrr = rr->right;
  Tree *rl = r->left;
  Tree *rrl = rr->left;
  Tree *rrrl = rrr->left;

  bool lhs = *((bool *)rl);
  bool rhs = *((bool *)rrrl);
  char opr = *((char *)rrl);

  bool *value = (bool *)calloc(1, sizeof(bool));

  if (opr == '^') {
    *value = lhs && rhs;
  } else if (opr == 'v') {
    *value = rhs || lhs;
  } else {
    printf("unknown operator\n");
  }

  return value;
}

void main() {
  Variable(Proposition);
  Variable(UnaryOperator);
  Variable(BinaryOperator);
  Variable(UnaryExpression);
  Variable(BinaryExpression);
  Variable(Expression);

  // Production rules.

  Production(Proposition, Map(evaluateProposition, Match("[TF]")));

  Production(UnaryOperator, Match("¬"));

  Production(BinaryOperator, Match("[\\^v]"));

  Production(UnaryExpression, And(UnaryOperator, Expression));

  Production(BinaryExpression,
             Map(evaluateBinaryExpression,
                 And(Match("\\("),
                     And(Expression,
                         And(BinaryOperator, And(Expression, Match("\\)")))))));

  Production(Expression,
             Or(Proposition, Or(UnaryExpression, BinaryExpression)));

  //@chatgpt
  while (true) {
    printf("> ");

    // Read user input from the console
    char input[256];
    scanf("%255s", input);

    // Construct a parse tree with the input string and NULL data
    Tree *parse = Parse(input, NULL);

    // Parse the input string using the grammar tree
    Tree *result = parseTree(Expression, parse);

    if (result == NULL) {
      // Parsing failed
      printf("error: Syntax error.\n");
    } else {
      // parsePrint(result);
      printf("< %s\n", *((bool *)result->right) ? "T" : "F");
    }
  }
}
