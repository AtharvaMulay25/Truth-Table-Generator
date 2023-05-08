//Author: AtharvaMulay25
//Date:  Aug, 2022   
//Title: This program draws the truth-table for a given boolean expression.

/*
    Symbols used in program:         \', + , . , (, ), ()', ^  
    Supported boolean operators:    NOT, OR, AND, XOR, NAND, NOR, XNOR
    Associativity:                  Left to right associtivity used in this program           
*/

#include <bits/stdc++.h>
using namespace std;
#define M 32
long int nofvar;
vector<vector<int>> truth_values; //made this global, after then I am resizing the size.    
set<char> input_variables;
map<char, int> bitValues;   

string parseString(string);
string toPostFix(string);
void setBitValues(long int);
int evaluatePostFix(string);
void printTruthTable();

int main()
{
    string function;
    cout << "Write a boolean expression F to get its truth-table: ";
    getline(cin, function);

    string inFix = parseString(function);
    string postFix = toPostFix(inFix);

    nofvar = input_variables.size();
    // nofvar=3;
    cout << "No.of variables in the truth-table: " << nofvar << endl;
    truth_values.resize(pow(2, nofvar));


    //Filling the truth values of all possible worlds in the truth-table
    for (long int i = 0; i < pow(2, nofvar); i++)
    {
        setBitValues(i);
        int truth_value = evaluatePostFix(postFix);
        truth_values[i].push_back(truth_value);
        // for(int j=0;j<truth_values[i].size();j++)  //for debugging
        // {
        //     cout<<truth_values[i][j]<<" ";
        // }
        // cout<<endl;
    }

    // Finally printing truth table on terminal
    printTruthTable();
   
    return 0;
}

//removes white spaces from boolean expr and converts expr to infix string
string parseString(string toParse)
{

    // removing all the white spaces
    for (int i = 0; i < toParse.size(); i++)
    {
        if (toParse[i] == ' ')
        {
            toParse.erase(toParse.begin() + i);
            i--;
        }
        if (isalpha(toParse[i]))
        {
            input_variables.insert(toParse[i]);
        }
    }

    // Converting to Infix string
    for (int i = 0; i < toParse.size() - 1; i++)
    {

        if ((isalpha(toParse[i]) && isalpha(toParse[i + 1]))) // a.b
        {
            toParse.insert(toParse.begin() + i + 1, 1, '.');
        }
        else if ((toParse[i] == '\'' && isalpha(toParse[i + 1]))) // a'.c
        {
            toParse.insert(toParse.begin() + i + 1, 1, '.');
        }
        else if (toParse[i] == ')' && toParse[i + 1] == '(') // ).(
        {
            toParse.insert(toParse.begin() + i + 1, 1, '.');
        }
        else if (toParse[i] == ')' && isalpha(toParse[i + 1])) // ).a
        {
            toParse.insert(toParse.begin() + i + 1, 1, '.');
        }
        else if (isalpha(toParse[i]) && toParse[i + 1] == '(') // a.(
        {
            toParse.insert(toParse.begin() + i + 1, 1, '.');
        }
        else if (toParse[i] == '\'' && toParse[i + 1] == '(') // a'.(
        {
            toParse.insert(toParse.begin() + i + 1, 1, '.');
        }
    }
    // cout << "inputvariables size: " << input_variables.size() << endl;
    // cout << "infix string: " << toParse << endl;

    return toParse;
}

//converts infix string to postfix string
string toPostFix(string inFix)
{
    string postfix;
    stack<char> operators;
    map<char, int> precedence = {{'\'', 5}, {'.', 4}, {'^',3}, {'+', 2}, {'(', 0}};
    
    //making postfix string
    for (auto x : inFix)
    {
        if (isalnum(x))
        {
            postfix.push_back(x);
        }
        else if (x == '(')
        {
            operators.push(x);
        }
        else if (x == '\'')
        {
            postfix.push_back(x);
        }
        else if (x == ')')
        {
            while (operators.top() != '(' && !operators.empty())
            {
                postfix.push_back(operators.top());
                operators.pop();
            }
            if (!operators.empty() && operators.top() == '(')
            {
                operators.pop();
            }
        }
        else
        {
            while (!operators.empty() && (precedence[x] <= precedence[operators.top()]))
            {
                postfix.push_back(operators.top());
                operators.pop();
            }
            operators.push(x);
        }
    }

    // removing all remaining characters from the operator stack and moving to postfix
    while (!operators.empty())
    {
        postfix.push_back(operators.top());
        operators.pop();
    }
    // cout << "postfix string: " << postfix << endl;
    return postfix;
}

//set values of bits of all the variables for a particular world/row in the truth table
void setBitValues(long int world)
{
    bitset<M> bits(world);
    set<char>::reverse_iterator rit;

    int counter = 0;

    //setting bits value from last to elements in the set
    for (rit = input_variables.rbegin(); rit != input_variables.rend(); rit++)
    {
        bitValues[*rit] = bits[counter++];
    }

    //pushing all bits(this combination/world) in the vector 
    for (int i = counter - 1; i >= 0; i--)
    {
        truth_values[world].push_back(bits[i]);
    }
}

//evaluates the value of postfix string for a given set of bit values of variables (i.e for a particular row/world)
int evaluatePostFix(string postfix)
{
    stack<int> evalPostfix;

    //evaluating the postfix string using evalPostfix stack
    for (int i = 0; i < postfix.size(); i++)
    {
        if (isalpha(postfix[i]))
        {
            evalPostfix.push(bitValues[postfix[i]]);
        }
        else if (postfix[i] == '.')
        {
            int val1 = evalPostfix.top();
            evalPostfix.pop();
            int val2 = evalPostfix.top();
            evalPostfix.pop();
            evalPostfix.push(val1 & val2);
        }
        else if (postfix[i] == '+')
        {
            int val1 = evalPostfix.top();
            evalPostfix.pop();
            int val2 = evalPostfix.top();
            evalPostfix.pop();
            evalPostfix.push(val1 | val2);
        }
        else if (postfix[i] == '\'')
        {
            int val1 = evalPostfix.top();
            evalPostfix.pop();
            evalPostfix.push(1 - val1);
        }
        else if(postfix[i] == '^')
        {
            int val1 = evalPostfix.top();
            evalPostfix.pop();
            int val2 = evalPostfix.top();
            evalPostfix.pop();
            evalPostfix.push(val1^val2);
        }
    }

    // cout << "Evaluated ans: " << evalPostfix.top() << endl;
    return evalPostfix.top();
}


//prints the entire truth table.
void printTruthTable()
{

    //To print the truth-table in the following way.
    // +---+---+---+---+       //drawline contains "+---+---+---+\n"
    // | a | b | c | F |       //partition contains "| "
    // +---+---+---+---+       // getonNextLine contains "|\n"
    //

     //setting all drawing strings
    string drawLine, partition, getOnNextLine;
    drawLine.push_back('+');
    for (int i = 1; i <= (nofvar + 1); i++)
    {
        drawLine.append("---+");
    }
    drawLine.append("\n");
    partition.append("| ");
    getOnNextLine.append("|\n");
    //

    cout << "The truth-table of given function is: \n";
    // printing the first row (first 3 rows)
    cout << drawLine;
    for (auto variables : input_variables)
    {
        cout << partition << variables << " ";
    }
    cout << partition << "F " << getOnNextLine;
    cout << drawLine;
    //

    // printing the rest rows
    int rows = 0;  //rows stors the no. of worlds/ rows in the truth-table.
    for (int j = 0; j < pow(2, nofvar); j++)
    {   
        rows++;
        for (auto truth_value : truth_values[j])
        {
            cout << partition << truth_value << " ";
        }
        cout << getOnNextLine;
        cout << drawLine;
    }
    //

    cout << endl;
    cout<<"No of rows in the truth-table: "<<rows<<endl<<endl;
}