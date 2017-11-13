#include "SilentCompiler.h"
#include <iostream>
#include <fstream>
#include <vector>
//File reader

using namespace std;

class fileReader{
	public:
	fileReader()
	{

	}
	string readAllText(string path)
	{
		string output;
		ifstream file(path);
		output.assign((istreambuf_iterator<char>(file)),(istreambuf_iterator<char>()));
		return output;
	}
	vector<string> readAllLines(string path)
	{
		vector<string> output;
		ifstream source(path);
		//if(source.is_open())
		return output;
	}
	string readLine(string path)
	{
		string output;

		return output;
	}
};

//Compiler structures
enum silentTokenType
{
	//Symbols
	silentPlusToken,
	silentMinusToken,
	silentMultiplyToken,
	silentDivideToken,
	silentParenthesToken,

	//Structure
	silentClassToken,
	silentFunctionToken,
	silentStructToken,
	silentArrayToken,

	
	//Access
	silentPublicToken,
	silentPrivateToken,
	silentProtectedToken,

	//Data
	silentByteToken,
	silentIntegerToken,
	silentFloatToken,
	silentLongToken,
	silentDoubleToken,
	silentStringToken,
};

typedef struct silentToken
{
	silentTokenType type;
	std::string value;

}silentToken;


enum silentNodeType
{
	//Value
	silentCharacterNode,
	silentStringNode,
	silentIntegerNode,
	silentLongNode,
	silentFloatNode,
	silentDoubleNode,

	//Functionality
	silentExpressionNode,
	silentAdditionNode,
	silentSubtractionNode,
	silentMultiplicationNode,
	silentDivisionNode,
	silentAssignNode,
	silentReturnNode,

	//Structure
	silentIfNode,
	silentWhileNode,
	silentForNode,
	silentFunctionNode,
	silentMethodNode,
	silentClassNode,
};


typedef struct silentValueNode
{
	silentNodeType type;
	string value;
}silentValueNode;

typedef struct silentExpressionNode
{
	silentNodeType type;
	silentValueNode parameters[2];

}silentExpressionNode;

typedef struct silentFunctionNode
{
	
}silentFunctionNode;

//Helper functions
char silentTestLetter(char character)
{
	string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	for(int i = 0; i < letters.length(); i++)
	{
		if(character == letters[i])
		{
			return 1;
			break;
		}
	}
	return 0;
}

char silentTestNumber(char character)
{
	string numbers = "1234567890";
	for(int i = 0; i < numbers.length(); i++)
	{
		if(character == numbers[i])
		{
			return 1;
			break;
		}
	}
	return 0;
}

char silentTestWhitespace(char character)
{
	if(isspace(character))
	{
		return 1;
	}

	else
	{
		return 0;
	}
}

//Tokenizer

vector<silentToken> silentTokenize(string* source)
{

}

//Parser


//Transform


//Code Generation




string* silentCompile(string path)
{

	fileReader fr;
	string rawSource = fr.readAllText(path);//no AST
	vector<silentToken> tokens = silentTokenize(&rawSource);

}
