
enum StatementType { RETURN_STATEMENT, EXPRESSION_STATEMENT };
enum ExpressionType { EXPR_INTEGER };

typedef struct {
    int value;
} IntegerExpression;

typedef struct {
    enum ExpressionType type;
    union {
        IntegerExpression *integer_expression;
    };
} Expression;

typedef struct {
    Expression *value;
} ReturnStatement;

typedef struct {
    Expression *expression;
} ExpressionStatement;

typedef struct {
    enum StatementType type;
    union {
        ReturnStatement *return_statement;
        ExpressionStatement *expression_statement;
    };
} Statement;

typedef struct {
    Statement *statements;
    int numStatements;
} AST;

void add_statement(AST *ast, Statement *statement);

char *ast_to_string(AST *ast);
char *return_statement_to_string(ReturnStatement *return_statement);
char *expression_statement_to_string(ExpressionStatement *expression_statement);
char *expression_to_string(Expression *expression);
char *integer_expression_to_string(IntegerExpression *integer_expression);
