
//=====================================================
// File scanner.cpp written by: Group Number: 4
// Done by: Micah McCarver
//=====================================================


#include<iostream>
#include<fstream>
#include<string>
#include<cctype>
#include<cstdlib>
using namespace std;


// MYTOKEN DFA to be replaced by the WORD DFA
// RE:   dfa replaced by word dfa
 
bool word(string s)
{
  int state = 0; //declarations
  int charpos = 0;
  
  while (s[charpos] != '\0') //charpos of letters
    {
      if(state == 0 && (s[charpos] == 'b' ||s[charpos] == 'g' || s[charpos] == 'h' || s[charpos] == 'k' || s[charpos] == 'm' || s[charpos] == 'n' || s[charpos] == 'r'))
  state = 1;
      else 
      if (state == 0 && (s[charpos] == 'a' || s[charpos] == 'e' ||s[charpos] == 'i' || s[charpos] == 'o' || s[charpos] == 'u' || s[charpos] == 'I' || s[charpos] == 'E')) 
  state = 2;    
      else 
      if (state == 0 && (s[charpos] == 'd' || s[charpos] == 'j' || s[charpos] == 'w' || s[charpos] == 'y' || s[charpos] == 'z'))
  state = 3;   
      else 
      if (state == 0 && s[charpos] == 's')
  state = 4; 
      else 
      if (state == 0 && s[charpos] == 'c')
  state = 5;
      else 
      if (state == 0 && s[charpos] == 't')
  state = 6;
      else 
      if (state == 1 && (s[charpos] == 'a' || s[charpos] == 'e' ||s[charpos] == 'i' || s[charpos] == 'o' || s[charpos] == 'u' || s[charpos] == 'I' || s[charpos] == 'E'))   
  state = 2;
      else 
      if (state == 1 && s[charpos] == 'y')
  state = 3;
      else 
      if (state == 2 && s[charpos] == 'n')
  state = 0;
      else 
      if (state == 2 && (s[charpos] == 'b' ||s[charpos] == 'g' || s[charpos] == 'h' || s[charpos] == 'k' || s[charpos] == 'm' || s[charpos] == 'n' || s[charpos] == 'r'))
  state = 1;
      else 
      if (state == 2 && (s[charpos] == 'a' || s[charpos] == 'e' ||s[charpos] == 'i' || s[charpos] == 'o' || s[charpos] == 'u' || s[charpos] == 'I' || s[charpos] == 'E'))   
  state = 2;
      else 
      if (state == 2 && (s[charpos] == 'd' || s[charpos] == 'j' || s[charpos] == 'w' || s[charpos] == 'y' || s[charpos] == 'z'))
  state = 3;
      else 
      if (state == 2 && s[charpos] == 's')
  state = 4;
      else 
      if (state == 2 && s[charpos] == 'c')
  state = 5;
      else 
      if (state == 2 && s[charpos] == 't')
  state = 6;
      else 
      if (state == 3 && (s[charpos] == 'a' || s[charpos] == 'e' ||s[charpos] == 'i' || s[charpos] == 'o' || s[charpos] == 'u' || s[charpos] == 'I' || s[charpos] == 'E'))
  state = 2;
      else 
      if (state == 4 && (s[charpos] == 'a' || s[charpos] == 'e' ||s[charpos] == 'i' || s[charpos] == 'o' || s[charpos] == 'u' || s[charpos] == 'I' || s[charpos] == 'E'))
  state = 2;
      else 
      if (state == 4 && s[charpos] == 'h')
  state = 3;
      else 
      if (state == 5 && s[charpos] == 'h')
  state = 3;
      else 
      if (state == 6 && (s[charpos] == 'a' || s[charpos] == 'e' ||s[charpos] == 'i' || s[charpos] == 'o' || s[charpos] == 'u' || s[charpos] == 'I' || s[charpos] == 'E'))
  state = 2;
      else
      if (state == 6 && s[charpos] == 's')
  state = 3;
      else
    return(false);

      charpos++;
    }//end of while

  // where did I end up????
  if (state == 2 || state == 0) return(true);  //the final state is where one ends up
   else return(false);  //if not returb false
}

//======================================================================================================
//Add the PERIOD DFA here
//RE: implemented with while and if/else
bool period(string s)
{
  int state = 0;
  int charpos = 0;

  while (s[charpos] != '\0') //implement the period dfa
    {
      if(state == 0 && s[charpos] == '.')
  state = 1;
      else 
  return (false);

      charpos++;
    }

  if(state == 1) return (true);
   else return(false);
}

//======================================================================================================

// Update the tokentype to be WORD1, WORD2, PERIOD, ERROR, etc.
//these are the token types
enum tokentype {ERROR, WORD1, WORD2, PERIOD, VERB, VERBNEG, VERBPAST, VERBPASTNEG, 
IS, WAS, OBJECT, SUBJECT, DESTINATION, PRONOUN, CONNECTOR, EOFM};

//these are the token names
//string tokenName[30] = { }; for the display names of tokens
string tokenName[30] = { "ERROR", "WORD1", "WORD2", "PERIOD", "VERB", "VERBNEG", "VERBPAST", "VERBPASTNEG", 
"IS", "WAS", "OBJECT", "SUBJECT", "DESTINATION", "PRONOUN", "CONNECTOR", "EOFM"};

//limit of amt of reserved words
const int amtOfWords = 19;

//setting up tble with reserved words and amt of words
string reservedwords[amtOfWords] = 
{  "masu", "masen", "mashita", "masendeshita", "desu", "deshita", "o", "wa", "ni",
   "watashi", "anata", "kare", "konojo", "sore", "mata" "soshite", "shikashi", "dakura", "eofm" };

string wordType[amtOfWords] = 
{  "VERB", "VERBNEG", "VERBPAST", "VERBPASTNEG", "IS", "WAS", "OBJECT", "SUBJECT", "DESTINATION", 
   "PRONOUN", "PRONOUN", "PRONOUN", "PRONOUN", "PRONOUN", "CONNECTOR", "CONNECTOR", "CONNECTOR", 
   "CONNECTOR", "EOFM" };

ifstream fin;  //fin for file

//======================================================================================================

//matching the reserved words and returning its type 
void matchReserved(tokentype &a, string w)
{
  for(int i = 0; i<amtOfWords; i++)
  {
    if(w == reservedwords[i])
    {
      string type = wordType[i];
      for(int x = 0; x<16; x++)
      {
        if(type == tokenName[x])
        {
          a = static_cast<tokentype>(x); //static cast x
          return;
        }
      }
    }
  }
}

//======================================================================================================

// Scanner processes only one word each time it is called
//gives back token and word itself
int scanner(tokentype& a, string& w)
{
    
    //fin declared above now reading in
    fin >> w;
    bool result = true;

    /*Calling the token functions one after another (if-then-else)
     And generate a lexical error message if both DFAs failed.
     Let the token_type be ERROR in that case.*/

    if(w == "eofm")
    {
        //exit if reach need be
        exit(0);
    }
    else
    {
        result = period(w);
        if(result) //if result is true
        {
            a = PERIOD;
            return 1;
        }
        else
        {
            result = word(w);
            if(!result)
            {
                a = ERROR;
                cout << "Lexical error: " << w << " is not a valid token" << endl;
            }
            else
            {
                bool test = isupper(w[w.size()-1]);
                if(test)
                {
                    a = WORD2;
                }
                else
                {
                    /*Making sure WORDs are checked against the reservedwords list
                     If not reserved, token_type is WORD1 or WORD2.*/
                    a = WORD1;
                    matchReserved(a, w);
                }
            }
        }
    }
    
}

//======================================================================================================

// The temporary test driver to just call the scanner repeatedly  
// This will go away after this assignment
// DO NOT CHANGE THIS!!!!!! 
// Done by:  Rika
int main()
{
  tokentype thetype;
  string theword; 
  string filename;


  cout << "Enter the input file name: ";
  cin >> filename;

  fin.open(filename.c_str());

   while (true)
    {
      scanner(thetype, theword);  // call the scanner

       cout << "Type is:" << tokenName[thetype] << endl;
       cout << "Word is:" << theword << endl;   

    }

  fin.close();

}// end


