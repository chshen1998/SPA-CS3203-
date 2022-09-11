// This file is to be integrated with the QueryValidator, which takes in a vector<PqlTokens>
// The checks below are done on a vector<string> before it gets converted to vector<PqlTokens>
// But for the sake of OOP design and adhering to SOLID principles, we will abstract this out into another class.

void checkSyntax() {
    int i = 0;

    while (i < tokens.size()) {

        // If it does not start with a design entity, then it must be a select statement
        if (i == 0 && !checkIfDesignEntity(delimited_query[i])) {
            break;
        }

        if (delimited_query[i] == "Select" && tokens.back().type == TokenType::SEMICOLON) {
            tokens.push_back(PqlToken(TokenType::DECLARATION_END, ""));
            isCurrentStringSynonym = false;
            break;
        }


        // If declaration does not start with design entity
        else {
            throw "Invalid Query Syntax :: Declaration needs to start with a design entity";
        }

        // If its not a declaration, comma or semicolon, it must be a synonym
        if (!checkIfSynonym(delimited_query[i])) {
            throw "Invalid Query Syntax :: Declaration must contain only synonyms";
        }
    }

    // Tokenization of Select Keyword -- after ';' of the declaration portion, must be Select
        if (delimited_query[i] != "Select") {
            throw "Invalid Query Syntax:: 'Select' keyword must come immediately after Declaration";
        }

        /* Similarly, after 'Select' MUST BE a synonym
           UPDATE THIS IN THE FUTURE FOR ADVANCED PQL
           -- Will have BOOLEAN | elem | Tuple(Elem), but for now is just synonym for BASIC PQL
         */
        if (!checkIfSynonym(delimited_query[i])) {
            throw "Invalid Query Syntax:: 'Select' keyword must come immediately after Declaration";
        }


}