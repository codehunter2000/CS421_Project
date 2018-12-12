#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cstdlib>
#include<cctype>
using namespace std;

// INSTRUCTION:  copy and edit your parser.cpp to create this file.
// cp ../ParserFiles/parser.cpp .
// Complete all ** parts.
// --------------------------------------------------------

//=================================================
// File translator.cpp written by Group Number: * Group 4 *
//=================================================

// ----- Changes to the parser.cpp ---------------------

// ** Declare dictionary that will hold the content of lexicon.txt
// Make sure it is easy and fast to look up the translation
// Do not change the format or content of lexicon.txt 

// ** Additions to parser.cpp here:
//    getEword - using the current lexeme, look up the English word
//               in the Lexicon if it is there -- save the result   
//               in saved_E_word
//    gen(line_type) - using the line type,
//                     sends a line of an IR to translated.txt
//                     (saved_E_word or saved_token is used)

// ** Be sure to put the name of the programmer above each function

// ** Be sure to put the corresponding grammar 
//    rule with semantic routines
//    above each non-terminal function
// -------------------------------------------

// Parser.cpp: 

enum tokentype {
	ERROR, WORD1, WORD2, PERIOD, VERB, VERBNEG, VERBPAST, VERBPASTNEG,
	IS, WAS, OBJECT, SUBJECT, DESTINATION, PRONOUN, CONNECTOR, EOFM
};

//these are the token names
//string tokenName[30] = { }; for the display names of tokens
string tokenName[16] = { "ERROR", "WORD1", "WORD2", "PERIOD", "VERB", "VERBNEG", "VERBPAST", "VERBPASTNEG",
						 "IS", "WAS", "OBJECT", "SUBJECT", "DESTINATION", "PRONOUN", "CONNECTOR", "EOFM" };

//limit of amt of reserved words
const int amtOfWords = 19;

//setting up tble with reserved words and amt of words
string reservedwords[amtOfWords] =
{ "masu", "masen", "mashita", "masendeshita", "desu", "deshita", "o", "wa", "ni",
   "watashi", "anata", "kare", "konojo", "sore", "mata", "soshite", "shikashi", "dakara", "eofm" };

string wordType[amtOfWords] =
{ "VERB", "VERBNEG", "VERBPAST", "VERBPASTNEG", "IS", "WAS", "OBJECT", "SUBJECT", "DESTINATION",
   "PRONOUN", "PRONOUN", "PRONOUN", "PRONOUN", "PRONOUN", "CONNECTOR", "CONNECTOR", "CONNECTOR",
   "CONNECTOR", "EOFM" };

ifstream fin;  //fin for file


void story();
void sentence();
void afterSubject();
void afterNoun();
void afterObject();
void noun();
void verb();
void be();
void tense();

void syntax_error1(tokentype);
void syntax_error2(string);

tokentype next_token();
bool match(tokentype);

tokentype saved_token;
string saved_lexeme;
string filename;
bool tokenExists = false; //no starting token

void scanner(tokentype&, string&);

string getTranslation(string);
bool fillDictionary();
void getEword();
void gen(string);
string saved_E_word;


//** require no other input files!
//** syntax error EC requires producing errors.text of messages




//-----------------------PARSER FUNCTIONS--------------------------
//Done by: Micah
//<story> -> <sentence> { <sentence> }
void story()
{
	cout << "Processing <story>" << endl;
	cout << endl;

	sentence();
	while (next_token() != EOFM)
	{
		sentence();
	}
}
//Done by: Micah
//<sentence> -> [CONNECTOR] #getEword# #gen# <noun> #getEword# SUBJECT #gen# <afterSubject>
void sentence()
{
	cout << "Processing <sentence>" << endl;

	if (next_token() == CONNECTOR)
	{
		match(CONNECTOR);
		getEword();
		gen("CONNECTOR");
	}

	switch (next_token())
	{
	case WORD1: case PRONOUN:
		noun();
		getEword();
		match(SUBJECT);
		gen("ACTOR");
		afterSubject();
		break;
	default:
		syntax_error2("<sentence>");
		return;
	}
}
//Done by: Andrew
//<afterSubject> -> <verb> #getEword# #gen# <tense> #gen# PERIOD | <noun> #getEword# <afterNoun>
void afterSubject()
{
	cout << "Processing <afterSubject>" << endl;

	switch (next_token())
	{
	case WORD2:
		verb();
		getEword();
		gen("ACTION");
		tense();
		gen("TENSE");
		match(PERIOD);
		break;
	case WORD1: case PRONOUN:
		noun();
		getEword();
		afterNoun();
		break;
	default:
		syntax_error2("<afterSubject>");
		return;
	}
}
//Done by: Andrew
//<afterNoun> -> <be> #getEword# #gen# #gen# PERIOD | DESTINATION #gen# <verb> #getEword# #gen# <tense> #gen# PERIOD | OBJECT #gen# <afterObject>
void afterNoun()
{
	cout << "Processing <afterNoun>" << endl;

	switch (next_token())
	{
	case IS: case WAS:
		be();
		getEword();
		gen("DESCRIPTION");
		gen("TENSE");
		match(PERIOD);
		break;
	case DESTINATION:
		match(DESTINATION);
		gen("TO");
		verb();
		getEword();
		gen("ACTION");
		tense();
		gen("TENSE");
		match(PERIOD);
		break;
	case OBJECT:
		match(OBJECT);
		gen("OBJECT");
		afterObject();
		break;
	default:
		syntax_error2("<afterNoun>");
		return;
	}
}
//Done by: Andrew
//<afterObject> -> <verb> #getEword# #gen# <tense> #gen# PERIOD | <noun> #getEword# DESTINATION #gen# <verb> #getEword# #gen# <tense> #gen# PERIOD
void afterObject()
{
	cout << "Processing <afterObject>" << endl;

	switch (next_token())
	{
	case WORD2:
		verb();
		getEword();
		gen("ACTION");
		tense();
		gen("TENSE");
		match(PERIOD);
		break;
	case WORD1: case PRONOUN:
		noun();
		getEword();
		match(DESTINATION);
		gen("TO");
		verb();
		getEword();
		gen("ACTION");
		tense();
		gen("TENSE");
		match(PERIOD);
		break;
	default:
		syntax_error2("<afterObject>");
		return;
	}
}
//Done by: Andrew
//<noun> -> WORD1 | PRONOUN
void noun()
{
	cout << "Processing <noun>" << endl;

	switch (next_token())
	{
	case WORD1:
		match(WORD1);
		break;
	case PRONOUN:
		match(PRONOUN);
		break;
	default:
		syntax_error2("<noun>");
		return;
	}
}
//Done by: Andrew
//<verb> -> WORD2
void verb()
{
	cout << "Processing <verb>" << endl;

	switch (next_token())
	{
	case WORD2:
		match(WORD2);
		break;
	default:
		syntax_error2("<verb>");
		return;
	}
}
//Done by: Andrew
//<be> -> IS | WAS
void be()
{
	cout << "Processing <be>" << endl;

	switch (next_token())
	{
	case IS:
		match(IS);
		break;
	case WAS:
		match(WAS);
		break;
	default:
		syntax_error2("<be>");
		return;
	}
}
//Done by: Andrew
//<tense> -> VERBPAST | VERBPASTNEG | VERB | VERBNEG
void tense()
{
	cout << "Processing <tense>" << endl;

	switch (next_token())
	{
	case VERBPAST:
		match(VERBPAST);
		break;
	case VERBPASTNEG:
		match(VERBPASTNEG);
		break;
	case VERB:
		match(VERB);
		break;
	case VERBNEG:
		match(VERBNEG);
		break;
	default:
		syntax_error2("<tense>");
		return;
	}
}
//Done by: Gabriel
bool match(tokentype thetype)
{
	if (next_token() != thetype)
	{
		syntax_error1(thetype);
	}
	else
	{
		cout << "Matched " << tokenName[thetype] << endl;
		tokenExists = false;
		return true;
	}
}
//Done by: Gabriel
tokentype next_token()
{
	if (tokenExists == false)
	{
		scanner(saved_token, saved_lexeme);
		tokenExists = true;
	}
	return saved_token;
}
//Done by: Micah
void syntax_error1(tokentype thetype)
{
	cout << "SYNTAX ERROR: expected " << tokenName[thetype] << " but found " << saved_lexeme << "." << endl;
	exit(1);
}
//Done by: Micah
void syntax_error2(string parserFunction)
{
	cout << "SYNTAX ERROR: unexpected " << saved_lexeme << " found in " << parserFunction << "." << endl;
	exit(1);
}
//-----------------------SCANNER FUNCTIONS----------------------------

bool word(string s)
{
	int state = 0; //declarations
	int charpos = 0;

	while (s[charpos] != '\0') //charpos of letters
	{
		if (state == 0 && (s[charpos] == 'b' || s[charpos] == 'g' || s[charpos] == 'h' || s[charpos] == 'k' || s[charpos] == 'm' || s[charpos] == 'n' || s[charpos] == 'r'))
			state = 1;
		else
			if (state == 0 && (s[charpos] == 'a' || s[charpos] == 'e' || s[charpos] == 'i' || s[charpos] == 'o' || s[charpos] == 'u' || s[charpos] == 'I' || s[charpos] == 'E'))
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
								if (state == 1 && (s[charpos] == 'a' || s[charpos] == 'e' || s[charpos] == 'i' || s[charpos] == 'o' || s[charpos] == 'u' || s[charpos] == 'I' || s[charpos] == 'E'))
									state = 2;
								else
									if (state == 1 && s[charpos] == 'y')
										state = 3;
									else
										if (state == 2 && s[charpos] == 'n')
											state = 0;
										else
											if (state == 2 && (s[charpos] == 'b' || s[charpos] == 'g' || s[charpos] == 'h' || s[charpos] == 'k' || s[charpos] == 'm' || s[charpos] == 'n' || s[charpos] == 'r'))
												state = 1;
											else
												if (state == 2 && (s[charpos] == 'a' || s[charpos] == 'e' || s[charpos] == 'i' || s[charpos] == 'o' || s[charpos] == 'u' || s[charpos] == 'I' || s[charpos] == 'E'))
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
																	if (state == 3 && (s[charpos] == 'a' || s[charpos] == 'e' || s[charpos] == 'i' || s[charpos] == 'o' || s[charpos] == 'u' || s[charpos] == 'I' || s[charpos] == 'E'))
																		state = 2;
																	else
																		if (state == 4 && (s[charpos] == 'a' || s[charpos] == 'e' || s[charpos] == 'i' || s[charpos] == 'o' || s[charpos] == 'u' || s[charpos] == 'I' || s[charpos] == 'E'))
																			state = 2;
																		else
																			if (state == 4 && s[charpos] == 'h')
																				state = 3;
																			else
																				if (state == 5 && s[charpos] == 'h')
																					state = 3;
																				else
																					if (state == 6 && (s[charpos] == 'a' || s[charpos] == 'e' || s[charpos] == 'i' || s[charpos] == 'o' || s[charpos] == 'u' || s[charpos] == 'I' || s[charpos] == 'E'))
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
		if (state == 0 && s[charpos] == '.')
			state = 1;
		else
			return (false);

		charpos++;
	}

	if (state == 1) return (true);
	else return(false);
}

//======================================================================================================

//matching the reserved words and returning its type 
void matchReserved(tokentype &a, string w)
{
	for (int i = 0; i < amtOfWords; i++)
	{
		if (w == reservedwords[i])
		{
			string type = wordType[i];
			for (int x = 0; x < 16; x++)
			{
				if (wordType[i] == tokenName[x])
				{
					a = static_cast<tokentype>(x); //static cast x
					return;
				}
			}
			return;
		}
	}
	return;
}

//======================================================================================================

// Scanner processes only one word each time it is called
//gives back token and word itself
void scanner(tokentype& a, string& w)
{

	//fin declared above now reading in
	fin >> w;
	cout << "Scanner called using word: " << w << endl;
	bool result = true;

	/*Calling the token functions one after another (if-then-else)
	 And generate a lexical error message if both DFAs failed.
	 Let the token_type be ERROR in that case.*/

	if (w == "eofm")
	{
		//exit if reach need be
		exit(0);
	}
	else
	{
		result = period(w);
		if (result) //if result is true
		{
			a = PERIOD;
			return;
		}
		else
		{
			result = word(w);
			if (!result)
			{
				a = ERROR;
				cout << "Lexical error: " << w << " is not a valid token" << endl;
			}
			else
			{
				bool test = isupper(w[w.size() - 1]);
				if (test)
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


// ------------------------------------------
// End of Parser.cpp


// Start of Translator additions
// ------------------------------------------


// GLOBALS (Gabriel -- added 12/7/18):

/* Note for dictionary: japasnese words will be listed first in the dictionary,
		the next imediate element after will be the english word*/
vector<string> dictionary;
ifstream lexIn;
ofstream translated;



// METHODS (Gabriel -- added 12/7/18)
string getTranslation(string japWord)
{
	string englishWord;
	string temp;
	for (int i = 0; i < dictionary.size(); i++)
	{
		temp = dictionary[i];
		if (temp == japWord)
		{
			englishWord = dictionary[i + 1];
			return englishWord;
		}
	}
	return NULL;
}

bool fillDictionary()
{
	string japWord, engWord;
	try
	{
		lexIn.open("lexicon.txt");
		//lexIn.open("C:/Users/gabri/Documents/GitHub/CS421_Project/TranslatorFiles/lexicon.txt");
		while (!lexIn.eof())
		{
			lexIn >> japWord;
			lexIn >> engWord;
			dictionary.push_back(japWord);
			dictionary.push_back(engWord);
		}
		lexIn.close();
		return true;
	}

	catch (ifstream::failure e)
	{
		cout << "Problem reading from lexicon.text (see fillDictionary method)" << endl;
		return false;
	}
}


void getEword()
{
  auto search = dictionary.find(saved_lexeme);  
  //dictionary searched through for J word
  
  //if found save the english equivalent
  if(search != dictionary.end()) 
    {
      saved_E_word = search -> second;
      return;
    } 

  //save the japanese word if you need
  saved_E_word = saved_lexeme; 
} 

void gen(string theType)
{
	if (theType == "TENSE")
	{
		cout << theType << " to " << tokenName[saved_token] << endl;
		translated << theType << " to " << tokenName[saved_token] << endl;
	}
	else
	{
		cout << theType << " to " << saved_E_word << endl;
		translated << theType << " to " << saved_E_word << endl;

	}
}

// The final test driver to start the translator
// Done by  * Gabriel Hunt *
int main()
{
	
	// Load the dictionary 
	bool fillSuccess = fillDictionary();
	if (fillSuccess)
		cout << "Dictionary filled successfully!" << endl;
	if (!fillSuccess)
	{
		cout << "Problem loading dictionary" << endl;
		return;
	}

	

	
  //** opens the output file translated.txt

	cout << "Enter the input file name: ";
	cin >> filename;
	fin.open(filename.c_str());

	translated.open("translated.txt");

  //** calls the <story> to start parsing
  story();
  //** closes the input file 
  //** closes traslated.txt
  fin.close();
  translated.close();
}// end



