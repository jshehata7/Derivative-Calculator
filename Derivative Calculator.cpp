#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

// Josiah Shehata -- Spring 2024: Final Project for CSC 1060 @ Front Range Community College

// Deliverables Used:
// 1. if-else/switch: Lines 182, 205, 211, 218, 224, 228, 244, 274, 316, 398, 413, 417, 420, 425, 433, 437, 448
// 2. loops: Lines 132, 193, 199, 252, 261, 309, 451
// 3. functions: Lines 37-40, 102, 125, 153, 382
// 4. array/vector: Lines 30-33, 38, 61-64, 130, 172, 395 
// 5. struct: Lines 21-24, 27-34
// 6. pointers: Lines 105-109
// 7. stringstream/external file: Lines 164, 190, 206, 212, 263, 271, 276, 282, 298, 310, 313, 332, 346-366
// 8. exceptions: Lines 74-84, 185

// Store user's inputs - JS
struct InputFunction {
    string independentVar;
    string placeHolder;
}; // end struct

// Store and organize parser data - JS
struct FunctionParser {
    bool allConstants = true;
    int independentVarCounter = 0;
    vector<long long int> actualCoefficients;
    vector<long long int> actualExponents;
    vector<string> operatorHolder;
    vector<string> newFunction; 
}; // end struct

// Function Declarations; ordered chronologically - JS
InputFunction PromptUser();
vector<string> ConvertToVector(string holderFunction);
FunctionParser ParseUserFunction(vector<string> userFunction, string independentVar);
void ComputeDerivative(vector<long long int> foundCoefficients, vector<long long int> foundPowers, vector<string> foundOperators, int VarCounter, bool allZero, string indepVar);

// Input: The user inputs an independent variable and a polynomial using that independent variable.
// Process: Prompt the user to enter an independent variable, then prompt the user to input a polynomial corresponding to that independent variable.
//          Having taken in both of the user's inputs as strings, convert the latter input, the polynomial, into a vector.
//          Then iterate through the vector verifying that it is the correct format. If it isn't, throw an error.
//          Then iterate through the vector again to find "important" points.
//          If the current vector element is the independent variable, look to the left of it to find coefficients.
//          If the current vector element is a caret, look to the right of it to find powers.
//          Store operators, coefficients, and powers in three seperate vectors.
//          For the derivative calculation, take each term with its corresponing coefficient and power and multiple them together, then decrement the power by one.
//          To print the derivative function, store all the coefficients, powers, operators, and blank spaces into a new vector. 
// Output:  Output critical steps of the parser to an external file. Then, output the computed derivative back to the user. 
//          If the user enters an invalid input, output an error message.
int main() {
      
  // Declarations
  bool constFunction;
  InputFunction userData;
  FunctionParser readUser;
  string holdIndependentVar;
  vector<long long int> userCoefficients;
  vector<long long int> userExponents;
  vector<string> userOperators;
  vector<string> userFunction;

  // Call function to prompt user for input - JS
  userData = PromptUser();
  
  holdIndependentVar = userData.independentVar;
  
  // Call function to convert user's inputed string (the polynomial) into a vector - JS
  userFunction = ConvertToVector(userData.placeHolder);
  
  try {
      // Call function to parse through user's input - JS
      readUser = ParseUserFunction(userFunction, holdIndependentVar);
  } // end try
  
  // Catch inputs written in invalid formats - JS
  catch (runtime_error& excpt) {
      cout << "\n" << excpt.what() << endl;
      cout << "Please re-run Derivative Calculator to try again." << endl;
      return 1;
  } // end catch
  
  userCoefficients = readUser.actualCoefficients;
  userExponents = readUser.actualExponents;
  userOperators = readUser.operatorHolder;
  constFunction = readUser.allConstants;
  
  // Call function to compute and print user's input
  ComputeDerivative(userCoefficients, userExponents, userOperators, readUser.independentVarCounter, constFunction, holdIndependentVar);

  return 0;
} // end main


// Input: None.
// Process: Prompt the user for seperate inputs relating to an independent variable and a polynomial.
//          Then store those inputs under a structure. Finally, return the structure to the user.
// Output:  Return the structure holding the independent variable and polynomial to the user.
InputFunction PromptUser() {
    // Declarations
    InputFunction userInput;
    int *userNbr = new int; // Use of pointer - JS
    
    *userNbr = 1; // Dereference and initialize the pointer - JS
    
    cout << "Hello User " << *userNbr << endl;
    cout << "Welcome to Derivative Calculator!" << endl;
    cout << "Currently we handle polynomials with positive coefficients comprised of terms unified through addition and subtraction" << endl;
    cout << "Please input an independent variable:" << endl;
    getline(cin, userInput.independentVar);
    cout << "Please input your function in a format with spaces between terms and operators:" << endl;
    cout << "f(" << userInput.independentVar << ") = " ;
    getline(cin, userInput.placeHolder);
    delete userNbr; // Delete/clear pointer - JS
    return userInput;
}; // end InputFunction


// Input: Take in string that user inputted as a polynomial.
// Process: Iterate through each element of the string transeferring it to a vector.
// Output:  Return the vector holding to the polynomial to the user.
vector<string> ConvertToVector(string holderFunction) {
    // Declaration
    unsigned int ii = 0;
    unsigned int jj = 0;
    string temp;
    vector<string> convertedFunction;
    
    for (ii = 0; ii < holderFunction.size(); ii++) {
      temp = holderFunction.at(ii);
      convertedFunction.push_back(temp);
    } // end for  
    return convertedFunction; 
}; // end ConvertToVector


// Input: Take in vector holding the polynomial and the independent variable 
// Process: Iterate through the vector twice. First time is to make sure it is in the correct format.
//         If it isn't, throw an error message and end program. Second time is to iterate through the polynomial looking for
//         "critical" elements. If an element corresponds to the independent variable, iterate a counter to keep track of that.
//         Then, look to elements to the left of the current one. This is to search for coefficients. Store these coefficients in
//         a seperate vector. If none are found, store a "1". If the current element is a caret, "^", look to the right of the current
//         element to search for powers. Store these in another vector. If none are found, store a "1". Lastly, if an operator is found 
//         (a "+" or a "-"), store it in another vector. Also, for each step of the parser process, output a statement corresponding to 
//         the current element of the polynomial to an external file. At the end of iterating through the polynomial, output the total 
//         elements in the coefficients', powers', and operators' vectors. This part is also outputted to the external file. Return all 
//         these vectors to the user under the FunctionParser structure.
// Output: Return the FunctionParser structure to user. Inside the structure are the coefficients', powers' and operators' vectors, as
//         as well as the allConstants boolean, which indicates if the polynomial is comprised of just constants or not.
FunctionParser ParseUserFunction(vector<string> userFunction, string independentVar) {

  // Declarations
  FunctionParser originalFunction;
  bool varCounter = false;
  long long int actualCoefficient = 0;
  long long int actualExponent = 0;
  int coefficientCounter = 0;
  int newCounter = -1;
  int operatorCounter = 0;
  InputFunction userData;
  ofstream outFS; // Output file stream - JS
  string coefficientHold;
  string coElement;
  string currElement;
  string exponentHold;
  string nextElement;
  string previousElement;
  string tempVal;
  vector<string> coefficientHolder;
  unsigned int ii;
  unsigned int jj;
  unsigned int kk;
  unsigned int mm;
  unsigned int nn;
  unsigned int tt;
  
  // Verify function is in valid format - JS
  for (tt = 0; tt < userFunction.size(); tt++) {
      if ( (userFunction.back() == "+") || (userFunction.back() == "-") || // Make sure user didn't just leave a "+" or "-" at the end of their polynomial - JS
         ( (userFunction.at(tt) == "+" || userFunction.at(tt) == "-") && (userFunction.at(tt + 1) != " "))   // Make sure user placed a space before and after each operator - JS 
         ) {
          throw runtime_error("Your function is in an invalid format");
      } // end if 
  } // end for
  
  // Open external file - JS
  outFS.open("ParserData.txt");
  
  // Print user's polynomial to external file - JS
  for (mm = 0; mm < userFunction.size(); mm++) {
      outFS << userFunction.at(mm);
  } // end for
  outFS << ":" << endl << endl;
  
  //Parsar -- Parse through user's function - JS
  for (ii = 0; ii < userFunction.size(); ii++) {
      jj = ii;
      
      currElement = userFunction.at(ii);
   
      // Term finder -- Find "critical" elements of polynomial - JS
      if ( currElement == " ") {
         outFS << currElement << " is a blank space" << endl;
         originalFunction.newFunction.push_back(currElement); 
         newCounter += 1;
      } else {      
         // Look for independent variable - JS
         if ( currElement == independentVar) {
            outFS << currElement << " is an independent variable" << endl;
            originalFunction.newFunction.push_back(currElement);
            newCounter += 1;
            originalFunction.allConstants = false;  // Polynomial is not just all constants (no independent variables) - JS
            originalFunction.independentVarCounter++;
      
            if (currElement == userFunction.front()) {
              varCounter = true;
            } // end if
            
            // Find preceding coefficients - JS
            // Check to find if term of polynomial is just comprised of the independent variable - JS
            if ((varCounter == true && originalFunction.independentVarCounter == 1) || (userFunction.at(ii - 1)) == " " ) {
                coefficientHold = "1";
                actualCoefficient = stoi(coefficientHold);
                originalFunction.actualCoefficients.push_back(actualCoefficient); 
                if ((currElement == userFunction.back() && ii == userFunction.size() - 1)) {
                    actualExponent = 1;
                    originalFunction.actualExponents.push_back(actualExponent);
                    actualExponent = 0;
                 } else if (userFunction.at(ii + 1) == " ") {
                    actualExponent = 1;
                    originalFunction.actualExponents.push_back(actualExponent);
                    actualExponent = 0;
                 } // end if
            } else {
                kk = ii;
                previousElement = userFunction.at(kk - 1);
                // Find preceding coefficients; stop once an operator or blank space is hit - JS
                while ((previousElement != " ") && (previousElement != "+") && (previousElement != "-")) {
                    coefficientHolder.push_back(previousElement); // Store each coefficient element in a vector - JS
                    kk--;
                    if (kk == 0) {
                        previousElement = " ";
                    } else {
                        previousElement = userFunction.at(kk - 1);
                    } // end if
                } // end while
                                   
                // Currently, coefficient components are in reverse order. This loop corrects that. - JS    
                for (nn = 0; nn < coefficientHolder.size() / 2; nn++) {
                    tempVal = coefficientHolder.at(nn);
                    coefficientHolder.at(nn) = coefficientHolder.at(coefficientHolder.size() - 1 - nn);
                    coefficientHolder.at(coefficientHolder.size() - 1 - nn) = tempVal;
                } // end for
                  
                // Now in the correct format, each element is transferred from the temporary vector for holding 
                // string coefficients to the actual coefficient vector. In the process, they are converted from 
                // strings to integers. This is done so the actual derivative calucations are smooth. - JS 
                for (nn = 0; nn < coefficientHolder.size(); nn++) {
                    coElement = coefficientHolder.at(nn);
                    outFS << "\t" << coElement << " is a coefficient" << endl;
                    coefficientHold += coElement;
                    actualCoefficient = stoi(coefficientHold);
                    coefficientCounter++;
                } // end for       
                originalFunction.newFunction.insert(originalFunction.newFunction.begin() + newCounter, coefficientHold);
                newCounter += coefficientCounter;
                originalFunction.actualCoefficients.push_back(actualCoefficient); 
                outFS << "Current Element: " << newCounter << endl;
            
                // While we are finding coefficients, we also check to see if the power of our term is "1". - JS
                if ((currElement == userFunction.back()) && ii == userFunction.size() - 1) {
                   nextElement = "1";
                   outFS << "\t" << nextElement << " is a power" << endl;
                   exponentHold += nextElement;
                   actualExponent++;
                   originalFunction.actualExponents.push_back(actualExponent);
                } else if (userFunction.at(ii + 1) != "^") {
                   nextElement = "1";
                   outFS << "\t" << nextElement << " is a power" << endl;
                   exponentHold += nextElement;
                   actualExponent++;
                   originalFunction.actualExponents.push_back(actualExponent);
                }// end if
            } // end if 
                
            // Reinitialize the temporary holders for the next iteration. These were used 
            // for the processing the coefficients. - JS 
            coefficientHold.clear();
            actualCoefficient = 0; 
            exponentHold.clear();
            actualExponent = 0;
         
         // Now, look for powers greater than one (indicated with the presence of a caret). - JS 
         } else if ( currElement == "^") {
            outFS << currElement << " is an exponential" << endl;
            originalFunction.newFunction.push_back(currElement);
            newCounter += 1;
            
            // Find powers
            nextElement = userFunction.at(jj + 1);
            
            // Find succeeding power; stop once a blank space is hit, or the polynomial ends.
            // Like the coefficients, each element corresponding to a term of the power is held in a
            // temporary vector. Once all the terms are found, that vector is converted from a string
            // into an integer. That integer is then stored into the actual exponents vector. - JS
            while ((nextElement != " ") && (jj < userFunction.size() - 1) ) {
               outFS << "\t" << nextElement << " is a power" << endl;
               originalFunction.newFunction.push_back(nextElement);
               newCounter++;
               outFS << "Current Element: " << newCounter << endl;
               exponentHold += nextElement;
               jj++;
               if (jj < userFunction.size() - 1) { 
                  nextElement = userFunction.at(jj + 1);
               } else {
                  nextElement = " ";
               } // end if
            } // end while
            
            // Once again, clear the temporary holders used for exponent 
            // processing so that they are ready for the next term. - JS
            actualExponent = stoi(exponentHold); 
            originalFunction.actualExponents.push_back(actualExponent);
            exponentHold.clear();
            actualExponent = 0;
         
         // Lastly, look for operators ("+" or "-") - JS 
         } else if ( (currElement == "+") || (currElement == "-") || (currElement == "*") || (currElement == "/") ) {
            outFS << currElement << " is an operator" << endl; 
            originalFunction.newFunction.push_back(currElement);
            newCounter++;
            originalFunction.operatorHolder.push_back(currElement); // Like the coefficients and powers, store the operators in a vector - JS
            operatorCounter++;
         } // end if
      } // end if     
      
      // Just to be safe, clear the coefficient holder and counter again - JS
      coefficientHolder.clear();  
      coefficientCounter = 0;                                                                                       
   } // end for
   
   // Format parts of the external file containing all the parser data. - JS
   outFS << endl;
   outFS << "Number of elements in original function: " << newCounter << endl;
   outFS << "NOTE: Counter starts at zero. Constants are not counted." << endl << endl;
   outFS << "Coefficients:" << endl;
   for (ii = 0; ii < originalFunction.actualCoefficients.size(); ii++) {
      outFS << originalFunction.actualCoefficients.at(ii) << " ";
   } // end for
   outFS << endl;
   outFS << "Powers:" << endl;
   for (ii = 0; ii < originalFunction.actualExponents.size(); ii++) {
      outFS << originalFunction.actualExponents.at(ii) << " ";
   } // end for
   outFS << endl;
   outFS << "Operators: " << endl;
   for (ii = 0; ii < originalFunction.operatorHolder.size(); ii++) {
      outFS << originalFunction.operatorHolder.at(ii) << " ";
   } // end for
   outFS << endl;

   // Close external file - JS
   outFS.close();
   
   return originalFunction; // return vectors storing coefficients, powers, and operators - JS
}; // end ParseUserFunction


// Input: Take in vectors storing coefficients, powers, and operators found in user's original polynomial.
//         Also, take in variables holding the independent variable, information pertaining to
//         coefficients, and counters related to the independent variable.
// Process: The derivative of each term of the polynomial is computed seperately while being pieced together
//         into one single vector. When the Power Rule is activatived (the "else" branch of the if-else), 
//         the new coefficients and powers are calculated and held in temporary variables. These temporary
//         variables are then used to convert the integers into strings that are then stored into a new, single
//         vector used to hold all the terms, including operators, that are a part of the computed derivative.
//         The computed derivative is then printed to the user, as well as information relating to the parser.
// Output: None.
void ComputeDerivative(vector<long long int> foundCoefficients, vector<long long int> foundPowers, 
                       vector<string> foundOperators, int VarCounter, bool allZero, string indepVar) {

// Declarations 
   int derivativeCoefficient = 0;
   int derivativeExponent = 0;
   int derivativeOpCounter = 0;
   int operatorCounter;
   string derivativeHolderCo;
   string derivativeHolderEx;
   string derivativeHolderOp = "";
   unsigned int ii;
   unsigned int qq;
   vector<string> userDerivative;
  
   // Compute the derivative and store it a seperate vector. - JS
   if (VarCounter == 1 && foundCoefficients.at(0) == 1 && foundCoefficients.size() == 1 && foundPowers.at(0) == 1) {
       userDerivative.push_back("1");
       // If the user's polynomial was comprised of a singular term, with that just term being the independent variable,
       // then this branch would ensure that the derivative came out be "1". - JS
   } else {
       // Iterate through every element of the coefficients vector. The powers vector will always have just as many elements as the 
       // coefficient one, and the operators vector will always have one less element. Since this is true, as well as the fact that 
       // each term of a polynomial has a coefficient (even if it's an implicit "1"). We know that once we have iterated through our
       // loop and corrected pieced together the applicable components, we will have completed our calculation of our user's derivative. - JS
       for (qq = 0; qq < foundCoefficients.size(); qq++) {
      
          // Temporary holders used for the derivative calculations and operators (for reference, the math being computed is the "Power Rule"). - JS
          derivativeCoefficient = foundCoefficients.at(qq) * foundPowers.at(qq);
          derivativeExponent = foundPowers.at(qq) - 1;
          
          if (qq < foundOperators.size()) {
              derivativeHolderOp = foundOperators.at(qq);
          } // end if
      
          if (derivativeCoefficient > 0) {
              derivativeHolderCo = to_string(derivativeCoefficient); // Each new coefficient is being converted to a string so that it may be stored in the derivative vector. - JS
              userDerivative.push_back(derivativeHolderCo);
              if (derivativeExponent >= 1) {
                  userDerivative.push_back(indepVar);
              } //end if
          } // end if
      
          if (derivativeExponent > 1) {
              derivativeHolderEx = to_string(derivativeExponent); // Likewise, each new coefficient is also being converted to a string so that it too may be stored in the derivative vector. - JS
              userDerivative.push_back("^");
              userDerivative.push_back(derivativeHolderEx);
          } // end if
      
          // This if-statement ensures that the operators are placed in the derivative vector while also being formated correctly
          // (Meaning spaces before and after each operator). - JS
          if (derivativeHolderOp != "" && (&foundCoefficients.at(qq) != &foundCoefficients.back())) {
              userDerivative.push_back(" ");
              userDerivative.push_back(derivativeHolderOp);
              derivativeOpCounter++;
              if (derivativeOpCounter <= operatorCounter) {
                  userDerivative.push_back(" ");
              } // end if
              derivativeHolderOp = "";
          } // end if         
  
      } // end for
   } // end if

   // Print derivative to user with an additional ending message. - JS
   cout << "\nf'(x) = ";
   if (allZero == true) {
       cout << "0" << endl;
   } else {
       for (ii = 0; ii < userDerivative.size(); ii++) {
           cout << userDerivative.at(ii);
       } // end for
       cout << endl;
   } // end if
   cout << "\n\nThank you for choosing Derivative Calculator!" << endl;
   cout << "We hold that our answer is to your satisfaction!" << endl;
   cout << "\nIf you are curious about how we were able to parse your input, "
        << "\nPlease visit our parser file: \n\n\t\t\t\t\t\t\t\t\t\tParserData\n\nLocated inside your file explorer to find out more." << endl;
   cout << "\nWe hope you visit our calculator again!" << endl;
  
   // No return needed. - JS
}; // end ComputeDerivative

