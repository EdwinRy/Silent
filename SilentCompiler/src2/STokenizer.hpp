#pragma once
#include <iostream>
#include <vector>
namespace Silent
{
    enum class TokenType
    {
        // Primitives
        I8,
        UI8,
        I16,
        UI16,
        I32,
        UI32,
        I64,
        UI64,
        F32,
        F64,
        String,

        // Keywords
        Function,
        Method,
        If,
        IfNot,
        Else,
        For,
        While,
        Class,
        Return,
        New,
        Delete,
        Namespace,
        Import,
        // Export,
        Asm,

        // Arithmetic Symbols
        Add,
        Subtract,
        Multiply,
        Divide,

        // Comparison Operators
        Equal,
        NotEqual,
        LessThan,
        LessThanOrEqualTo,
        MoreThan,
        MoreThanOrEqualTo,

        // Logical Operators
        And,
        Or,
        Xor,
        Not,

        // Bit Shift Operators
        RightShift,
        LeftShift,

        // Assignment Operators
        Assign,
        AddAssign,
        SubtractAssign,
        MultiplyAssign,
        DivideAssign,

        // Other Symbols
        Semicolon,
        Comma,
        Fullstop,
        OpenParam,
        CloseParam,
        OpenBracket,
        CloseBracket,
        OpenScope,
        CloseScope,
        // ScopeResolution,

        // Other
        Identifier,
        NumberValue,
        StringValue,
        Unrecognised
    };

    class Token
    {
        public:
        TokenType type;
        std::string value;
        unsigned long long line;

        bool IsPrimitive();
        bool IsKeyword();
        bool IsArithmetic();
        bool IsComparison();
        bool IsLogic();
        bool IsBitShift();
        bool IsAssignment();
        bool IsOtherSymbol();
        bool IsOther();
    };


    class Tokenizer
    {
        private:
        std::string TokenizeString(
            std::string source, 
            unsigned long long &index
        );

        std::vector<Token> tokens;
        unsigned long long line;

        public:
        Tokenizer();
        bool Tokenize(std::string source);
        std::vector<Token> GetTokens();

    };
}