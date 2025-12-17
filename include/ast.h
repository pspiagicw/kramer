
enum StatementType { RETURN_STATEMENT };

typedef struct {
} ReturnStatement;

typedef struct {
  enum StatementType type;
  union {
    ReturnStatement return_statement;
  };
} Statement;

typedef struct {
  Statement *statements;
  int numStatements;
} AST;
