#ifndef __PARSER_COPY_HPP__
#define __PARSER_COPY_HPP__

#include "CLArg.hpp"
#include <vector>
#include <cstring>
#include "Fork.hpp"
#include "WaitPID.hpp"
//#include "Execvp.hpp"
#include "Test.hpp"
#include "Exit.hpp"
//#include "UserFunc.hpp"
#include "RParen.hpp"
#include "LParen.hpp"
#include "CLBase.hpp"
#include "CLSpec.hpp"
#include "CLCon.hpp"
#include "CLFunc.hpp"
class Parser {
	private:
		std::string parsedString = "";
  	public:
	Parser(){}
	Parser(std::string userI)
	{
		parsedString = userI;
	}
	CLBase* makeCLFunc(std::string cmd, std::vector<CLSpec*> los,std::string inRedir, std::string outRedir)
	{
		std::cout << "MakeCLFunc's OutRedir: '" << outRedir << "'\n"; 
		CLBase * retFunc;
		if(cmd == "test")
		{
			retFunc = new Test(cmd,los,inRedir, outRedir);
		}
		else if(cmd == "exit")
		{
			retFunc = new Exit(los);
		}
		else
		{	
			std::cout << "Making UserFunc...\n";
			retFunc = new CLFunc(cmd,los,inRedir,outRedir);
		}
		return retFunc;
	}
	std::vector<std::vector<int>> findParenPos(std::vector<CLBase*>& objs)
	{
		std::vector<int> LParenPos;
		std::vector<int> RParenPos;
		std::vector<std::vector<int>> retVector;
		std::vector<CLBase*>::iterator parenIterator = objs.begin();
		std::string curString;
		for(int i = 0; i < objs.size(); i++)
		{
			curString = objs.at(i)->getType();
			if(curString == "(")
			{
				LParenPos.push_back(i);
			}
			if(curString == ")")
			{
				RParenPos.push_back(i);
			}
			std::advance(parenIterator, 1);	
		}
		retVector.push_back(LParenPos);
		retVector.push_back(RParenPos);
		return retVector;
	}
	CLBase*  makeCLArg(std::vector<CLBase*> objs)
	{
		CLBase * ret;
		if(objs.empty())
		{
			ret = NULL;
			exit(-1);
			return ret;
		}
		//Insert 2 parenthesis at either end of the whole statement
		//Insures that the entire expression gets reduced
		CLBase* newLParen = new LParen();
		CLBase* newRParen = new RParen();
		objs.insert(objs.begin(),newLParen);
		objs.push_back(newRParen);
		//Determines how many parenthesis there are
		//Checks to see if the number of parenthesis are balanced or not
		std::vector<std::vector<int>> parenPos = findParenPos(objs);
		std::vector<int> LParenPos = parenPos.at(0);
		std::vector<int> RParenPos = parenPos.at(1);
	//	std::cout << "The # of LParens are: " << LParenPos.size() << "\n";
	//	std::cout << "The # of RParens are: " << RParenPos.size() << "\n";
		if(LParenPos.size() != RParenPos.size())
		{
			perror("Uneven number of left and right parenthesis\n");
			system("exit");
		}

		//Define new variables to use when parsing the vector
		int objsBetween = 0;
		int curRParen = 0;
		int curLParen = 0;
		int i = 1;
		CLBase* curObj;
		CLBase* nextObj;
		CLBase* nextNextObj;

		bool parenExist = true;	
		while(parenExist)
		{
	//		std::cin.get();
			std::cout << "Printing vector:...\n";
			this->printVector(objs);
			//Defines (again) the positions of all the parenthesis in the vector
			//Sets curLParen and curRParen which enclose the expression being reduced
			//Has different cases based on the number of objects between the current parenthesis group
			parenPos = findParenPos(objs);
               		LParenPos = parenPos.at(0);
                	RParenPos = parenPos.at(1);
        //    	std::cout << "The # of LParens are: " << LParenPos.size() << "\n";
          //     	std::cout << "The # of RParens are: " << RParenPos.size() << "\n";

			curRParen = RParenPos.at(0);
			i = 1;
			int curLParen = LParenPos.at(LParenPos.size()-i);
			while(curLParen > curRParen)
			{
				i++;
				curLParen = LParenPos.at(LParenPos.size()-i);	
			}
	//		std::cout << "Current Parenthesis pos = L:" << curLParen << " R:" << curRParen << "\n";
			objsBetween = curRParen - curLParen -1;
	//		std::cout << "Objects between is: " << objsBetween << "\n";
			curObj  = objs.at(curLParen+1); 
	//		std::cout << "getType of curObj is: " << curObj->getType() << "\n";
			if(objsBetween == 1)//if the innermost paren has 1 ojb
			{
				if((curObj->getType() == "cmd")||(curObj->getType()== "CLArg"))
				{	//delete right to left
					//Exit Condition: Delete the last pair of parenthesis
					if(LParenPos.size() == 1)
					{
						parenExist = false;
					}
	//				std::cout << "Erasing parenthesis...\n";
					objs.erase(objs.begin() + curRParen);
					objs.erase(objs.begin() + curLParen);
					
			//		LParenPos.erase(LParenPos.begin()+LParenPos.size()-i);
			//		RParenPos.erase(RParenPos.begin());
				}
				else
				{
					perror("objsBetween =1, is not cmd or CLArg\n");
					exit(-1);
				}
				//delete parenthesis
			}	
			else if(objsBetween == 2)
			{
				nextObj = objs.at(curLParen+2);
				if((curObj->getType() == "cmd")||(curObj->getType() == "CLArg"))
				{
					if(nextObj->getType() == ";")
					{	
						//Exit Condition: Delete the last pair of parenthesis
						if(LParenPos.size()==1)
						{
							parenExist = false;
						}
	//					std::cout << "Erasing parenthesis with semicolon...";
						objs.erase(objs.begin() + curRParen);
						objs.erase(objs.begin() + curLParen+ 2);
						objs.erase(objs.begin() + curLParen);
						
			//			LParenPos.erase(LParenPos.begin()+LParenPos.size()-i);
                          //             		RParenPos.erase(RParenPos.begin());

						//deletes the extraneous semicolon
					}
					else
					{
						perror("objsBetween = 2 Must be cmd+semicolon\n");
						exit(1);

					}
				}
				else
				{
					  perror("objsBetween = 2 Must be cmd+semicolon\n");
					exit(-1);
				}
				
			}// objs between = 2
			else if(objsBetween >= 3)
			{
				nextObj = objs.at(curLParen +2);
				nextNextObj = objs.at(curLParen +3);
				//must match form cmd+con+cmd
				bool inputGood = true;
				std::string temp = curObj->getType();
	//			std::cout << "3Objs: " << temp;
				if((temp != "cmd")&&(temp != "CLArg"))
				{
					inputGood = false;	
				}
				temp = nextObj->getType();
	//			std::cout << " " << temp;
				if((temp != ";")&&(temp != "||")&&(temp != "&&"))
                                {
                                        inputGood = false;
                                }
				temp = nextNextObj->getType();
	//			std::cout << " " << temp << "\n";
                                if((temp != "cmd")&&(temp != "CLArg"))
                                {
                                        inputGood = false;
                                }
				if(inputGood)
				{
					CLBase * newArg = new CLArg(curObj,nextObj,nextNextObj);
					objs.erase(objs.begin()+curLParen+3);
					objs.erase(objs.begin()+curLParen+2);
					objs.erase(objs.begin()+curLParen+1);

					//objs.erase(objs.begin()+curLParen+1,objs.begin()+3);
					objs.insert(objs.begin()+curLParen+1,newArg);
					//make CLArg
				}
				else
				{
					perror("3 objs: doesnt match form: cmd+con+cmd");
					exit(-1);
				}

			}//end objsBetween =3
			else
			{
				perror("Objs between is negative???\n");
				exit(-1);
			}	
	//		std::cout << "After deletion...\n";
	//		this->printVector(objs);	

		}//end of while loop LParen>0
		//objs no longer has any parenthesis, do in order
		
		return(objs.at(0));
		
	}//end of function
	void printVector(std::vector<CLBase*> a)
	{
		std::vector<CLBase*>::iterator baseIterator = a.begin();
		for(int i =0; i < a.size(); i++)
		{
			std::cout << (*baseIterator)->getType() << "\n";
			std::advance(baseIterator,1);
		}	
	}
	std::vector<CLBase*> parseInput()
        {
                return(this->parseInput(parsedString));
        }
	
    	std::vector<CLBase*> parseInput(std::string userInput)
	{
	std::stringstream testStream;
	testStream <<  userInput;
        char curChar = '\0';
        std::string cmd = "";
        std::string spec = "";
	std::string inputRedir = "";
	std::string outputRedir = "";
	//parsingConnector Variables
                        std::string connectorStr = "";
                        std::string connectorValue = "";
                        bool connectorSet = false;


	CLBase * parsedCMD;
        std::vector<CLSpec*> parsedSpecs;
	

        bool isNotParsingAnything = true;
        bool parsingCommand = false;
        bool parsingSpec = false;
	bool hasParsedSpec = false;
	bool parsingInputRedir = false;
	bool parsingOutputRedir = false;
	bool willParseInputRedir = false;
	bool willParseOutputRedir = false;
      
        bool parsingCon = false;
        bool parsingLParen = false;
        bool parsingRParen = false;
        std::vector<CLBase *> parsedObjs;

        while(true)//the current character  read in curChar
        {
		testStream.get(curChar);
		std::cout << "CurChar is: " << curChar << "\n";
		if((curChar == '#')||(testStream.eof()))//Unconditional exit conditions, must finish making and appending objects
		{
			if(parsingCommand)
			{
				std::cout << "goto parsingCOMMAND\n";
				goto EXIT_CONDITION_PARSING_COMMAND;
			}
			else if(parsingCon)
			{
				std::cout << "goto parsingCON\n";
				goto EXIT_CONDITION_PARSING_CON;
			}

			break;
		}

                //std::cout << "The curChar is: '" << curChar<< "' ";

		if(isNotParsingAnything)//Expects char to be a cmd or LParen			
		//Purpose: Redirects curChar to the right path
		//	Eliminates (ignores) preceding spaces
		{ 
			isNotParsingAnythingLabel:

			if(curChar == ' ')
			{}//do nothing ignores preceding spaces
			else if(curChar == '(')
			{
				parsingLParen = true;
				isNotParsingAnything = false;
			}
			else//is parsing a command
			{
				parsingCommand = true;
				isNotParsingAnything = false;
			}
		}
		if(parsingCommand)//cur char
		{
                        if((curChar == ' ')&&(parsingSpec == true))
                        {
                                struct CLSpec* parsedSpec = new CLSpec(spec);
                                std::cout << "Creating new spec: " << spec << "\n";
                                spec = "";
                                parsedSpecs.push_back(parsedSpec);
                  	//              parsingSpec = false;

                        }

			//set flags
                        if(curChar != ' ')
                        {
                                if(willParseInputRedir)
                                {
					parsingSpec = false;
                                        willParseInputRedir = false;
                                        parsingInputRedir = true;
                                }
                                else if(willParseOutputRedir)
                                {
					parsingSpec = false;
                                        willParseOutputRedir = false;
                                        parsingOutputRedir = true;
                                }
                        }

                     


			//Spec flags ' ' and '-'
			//Exit conditions: either connector or RParen next
			if((curChar == '|')||(curChar == '&')||(curChar == ';')||(curChar == ')'))
			{
				EXIT_CONDITION_PARSING_COMMAND:
				if((spec != "")&&(parsingSpec == true))
				{//finish making last spec
					struct CLSpec* parsedSpec2 = new CLSpec(spec);
			              std::cout << "Creating new spec: " << spec << "\n";

					spec = "";
					parsingSpec = false;
					parsedSpecs.push_back(parsedSpec2);
				}
	//			std::cout << "Making command: " << cmd << "\n";
				std::cout << "Output_redir = " << outputRedir << "\n";
				parsedCMD = makeCLFunc(cmd, parsedSpecs,inputRedir,outputRedir);
				parsedObjs.push_back(parsedCMD);
				parsedSpecs.clear();//clears out local variable
				cmd = "";
				inputRedir = "";
				outputRedir = "";
				parsingSpec = false;	
				parsingCommand = false;
				hasParsedSpec = false;
				parsingInputRedir = false;
				parsingOutputRedir = false;
				if(testStream.eof()||curChar == '#')
				{
					break;
				}
				if(curChar == ')')
				{
					parsingRParen = true;
				}
				else
				{
					parsingCon = true;
				}

			}
		
			else if(parsingSpec)
			{
				if(curChar == '<')
				{
					inputRedir = "";
					willParseInputRedir = true;
					//	parsingInputRedir = true;
					parsingSpec = false;
				}
				else if(curChar == '>')
				{
					outputRedir = "";
					willParseOutputRedir = true;
					parsingSpec = false;
				}
				else if(curChar != ' ')
				{
					spec+=curChar;
				}
				else
				{}//do nothing space
			}
			else if(parsingInputRedir)
			{
				if(curChar != ' ')
				{
					inputRedir += curChar;
				}
				else
				{
					parsingInputRedir = false;
					parsingSpec = true;
				}
			}
			else if(parsingOutputRedir)
			{
				if(curChar != ' ')
				{
					outputRedir += curChar;
				}
				else
				{
					parsingOutputRedir = false;
					parsingSpec = true;
				}
			}
			else if(curChar == ' ')
			{
				hasParsedSpec = true;
				parsingSpec = true;
			}
			else if(!hasParsedSpec)
			{
				cmd+= curChar;
				
			}
			else
			{
			//	std::cout << "Is this a space?: " << curChar << "\n";
			//	parsingSpec = true;
			}
		
			
		}//end parsingCMD
		if(parsingRParen)//RParen is followed by Con or RParen
				//Must handle spaces because it doesn't go through isNotParsingAnything
		{
			if(curChar == ')')
			{
				CLBase* rParen = new RParen();
				parsedObjs.push_back(rParen);		
			}
			else if(curChar == ' '){}//do nothing, do not parse space
			else if((curChar == '|')||(curChar == '&')||(curChar == ';'))//exit condition: connector is next
			{
				parsingCon = true;
				parsingRParen = false;		
			}
			else	//do nothing, discard invalid input?
			{
			
			
				perror("Closing Parenthesis must be followed by a connector)");
				exit(-1);
			}	
		}//end parsingRParen
		if(parsingCon)
		{
			
    
			if((!connectorSet)&&((curChar == '|')||(curChar == '&')||(curChar == ';')))
			{
				/*Version1: Good Connector Assumed. * If encountering a connector, assumes it's valid, ignore rest of input until the next space*/
				connectorSet = true;
				if(curChar == '|')
				{
					connectorStr = "vertical bar";
					connectorValue = "||";
				}
				else if(curChar == '&')
				{
					connectorStr = "ambersand";
					connectorValue = "&&";
				}
				else if(curChar == ';')
				{
					connectorStr = "semicolon";
					connectorValue = ";";
				}
				else{perror("Connector Parse Error\n");}
				
					
				
				/*Version2: Error Checking
				if(prevCon != ' ')//if not set, then set
				{
					prevCon = curChar;
				}
				else if((curChar == '|')&&(prevCon == '|'))
				{}//valid
				else if((curChar == '&')&&(prevCon == '&'))
				*/
			}//end settingCon
			else if(curChar == ' ')//exit condition
			{
				EXIT_CONDITION_PARSING_CON:
				CLBase* parsedCon = new CLCon(connectorStr, connectorValue);
                                parsedObjs.push_back(parsedCon);
				connectorStr = "";//reset state variables
				connectorValue = "";
				connectorSet = false;
				parsingCon = false;
				//parses and ignores the space
				isNotParsingAnything = true;
				if(testStream.eof()||curChar == '#')
				{
					break;
				}

			}
			else{}//do nothing, discard additional input if connector is set
			
		}//end parsingCon
		if(parsingLParen)
		{
			if(curChar == '(')
			{
				CLBase * parsedLParen = new LParen();
				parsedObjs.push_back(parsedLParen);
			}

			else if(curChar == ' ')
			{}	//do nothing, discard space
			else //if there is a command to be parsed
			{
				parsingLParen = false;
				isNotParsingAnything = true;
				goto isNotParsingAnythingLabel;	
			}
		}//end parsingLParen
	//	if(parsingSpec) {std::cout << "'spec'";}
	//	if(parsingCommand) {std::cout << "'cmd'";}
	//	if(parsingCon)	{std::cout<<"'con'";}
	//	if(parsingLParen) {std::cout << "'('";}
	//	if(parsingRParen) {std::cout << "')'";}
	//	std::cout << "\n";
		
			
	}//end while loop
	return parsedObjs;
		
	}//end function	
    };
#endif
