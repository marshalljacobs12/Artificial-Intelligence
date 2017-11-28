#include "ClueReasoner.h"
using namespace std;

int ClueReasoner::GetPlayerNum(string player)
{
	if (player == case_file)
		return num_players;
	
	for (int i = 0; i < num_players; i++)
		if (player == players[i])
			return i;
			
	cout<<"Illegal player: "<<player<<endl;
	return -1;
}
int ClueReasoner::GetCardNum(string card)
{
	for (int i = 0; i < num_cards; i++)
		if (card == cards[i])
			return i;
			
	cout<<"Illegal card: "<<card<<endl;
	return -1;
}

string ClueReasoner::QueryString(int return_code)
{
	if (return_code == kFalse)
		return "n";
	else if (return_code == kTrue)
		return "Y";
	else
		return "-";
}

void ClueReasoner::PrintNotepad()
{
	for (int i = 0; i < num_players; i++)
		cout<<"\t"<<players[i];
	cout<<"\t"<<case_file<<endl;
	
	for (int i = 0; i < num_cards; i++)
	{
		cout<<cards[i]<<"\t";
		for (int j = 0; j < num_players; j++)
			cout<<QueryString(Query(players[j], cards[i]))<<"\t";
		
		cout<<QueryString(Query(case_file, cards[i]))<<endl;
	}
}
	
void ClueReasoner::AddInitialClauses()
{
	/* The following code is given as an example to show how to create Clauses and post them to the solver. SatSolver.h uses the following typedefs:
		typedef int Literal;
		typedef std::vector<Literal> Clause;
		
	That is, a Literal (a propositional variable or its negation) is defined as a positive or a negative (meaning that it is in negated form, as in -p or -q) integer, and a Clause is defined as a vector of Literals.
	
	The function GetPairNum(p, c) returns the literal that corresponds to card c being at location p (either a player's hand or the case_file). 
	See ClueReasoner.h, lines 7-29 for a definition of the arrays and variables that you can use in your implementation. 
	*/

	// Each card is in at least one place (including the case file).
	for (int c = 0; c < num_cards; c++)	// Iterate over all cards.
	{
		Clause clause;
		for (int p = 0; p <= num_players; p++)	// Iterate over all players, including the case file (as a possible place for a card).
			clause.push_back(GetPairNum(p, c));
		
		solver->AddClause(clause);
	}
        
	// If a card is in one place, it cannot be in another place.
	// TO BE IMPLEMENTED AS AN EXERCISE
	for (int c = 0; c < num_cards; c++) 
	{
		for (int p1 = 0; p1 <= num_players; p1++) 
		{
			for (int p2 = p1 + 1; p2 <= num_players; p2 ++) 
			{
				Clause clause;
				clause.push_back(-GetPairNum(p1, c));
				clause.push_back(-GetPairNum(p2, c));
				solver->AddClause(clause);
			}
		}
	}

	// At least one card of each category is in the case file.
	// TO BE IMPLEMENTED AS AN EXERCISE
	//suspects
	Clause suspects_clause;
	for (int i = 0; i < num_suspects; i++) {
		suspects_clause.push_back(GetPairNum(case_file, suspects[i]));
	}
	solver->AddClause(suspects_clause);

	//weapons
	Clause weapons_clause;
	for (int i = 0; i < num_weapons; i++) {
		weapons_clause.push_back(GetPairNum(case_file, weapons[i]));
	}
	solver->AddClause(weapons_clause);

	//rooms
	Clause rooms_clause;
	for (int i = 0; i < num_rooms; i++) {
		rooms_clause.push_back(GetPairNum(case_file, rooms[i]));
	}
	solver->AddClause(rooms_clause);

	// No two cards in each category can both be in the case file.
	// TO BE IMPLEMENTED AS AN EXERCISE

	//suspects
	for (int s1 = 0; s1 < num_suspects; s1++) {
		for (int s2 = s1 + 1; s2 < num_suspects; s2++) {
			Clause clause;
			clause.push_back(-GetPairNum(case_file, suspects[s1]));
			clause.push_back(-GetPairNum(case_file, suspects[s2]));
			solver->AddClause(clause);
		}
	}

	//weapons
	for (int w1 = 0; w1 < num_weapons; w1++) {
		for (int w2 = w1 + 1; w2 < num_weapons; w2++) {
			Clause clause;
			clause.push_back(-GetPairNum(case_file, weapons[w1]));
			clause.push_back(-GetPairNum(case_file, weapons[w2]));
			solver->AddClause(clause);
		}
	}

	//rooms
	for (int r1 = 0; r1 < num_rooms; r1++) {
		for (int r2 = r1 + 1; r2 < num_rooms; r2++) {
			Clause clause;
			clause.push_back(-GetPairNum(case_file, rooms[r1]));
			clause.push_back(-GetPairNum(case_file, rooms[r2]));
			solver->AddClause(clause);
		}
	}
}

void ClueReasoner::Hand(string player, string cards[3])
{
	// GetPlayerNum returns the index of the player in the players array (not the suspects array). Remember that the players array is sorted wrt the order that the players play. Also note that, player_num (not to be confused with num_players) is a private variable of the ClueReasoner class that is initialized when this function is called.
	player_num = GetPlayerNum(player);
	
	// TO BE IMPLEMENTED AS AN EXERCISE
	for(int c = 0; c < 3; c++) {
		Clause clause;
		clause.push_back(GetPairNum(player, cards[c]));
		solver->AddClause(clause);
	}
}

void ClueReasoner::Suggest(string suggester, string card1, string card2, string card3, string refuter, string card_shown)
{
	// Note that in the Java implementation, the refuter and the card_shown can be NULL. 
	// In this C++ implementation, NULL is translated to be the empty string "".
	// To check if refuter is NULL or card_shown is NULL, you should use if(refuter == "") or if(card_shown == ""), respectively.
	
	// TO BE IMPLEMENTED AS AN EXERCISE
	//if nobody refutes the suggestion, then either the suggester or the case file has card1 card2 and card3
	if (refuter == "") {

		Clause card1_clause;
		card1_clause.push_back(GetPairNum(suggester, card1));
		card1_clause.push_back(GetPairNum(case_file, card1));
		solver->AddClause(card1_clause);

		Clause card2_clause;
		card2_clause.push_back(GetPairNum(suggester, card2));
		card2_clause.push_back(GetPairNum(case_file, card2));
		solver->AddClause(card2_clause);

		Clause card3_clause;
		card3_clause.push_back(GetPairNum(suggester, card3));
		card3_clause.push_back(GetPairNum(case_file, card3));
		solver->AddClause(card3_clause);
	}
	//somebody refuted the suggestion
	else {
		//player whose perspective we have is not the player shown the card by the refuter
		if (card_shown == "") {
			//refuter has card1, card2, or card3
			Clause refuter_clause;
			refuter_clause.push_back(GetPairNum(refuter, card1));
			refuter_clause.push_back(GetPairNum(refuter, card2));
			refuter_clause.push_back(GetPairNum(refuter, card3));
			solver->AddClause(refuter_clause);

		}
		//player whose perspective we have is shown the card by the refuter
		else {
			Clause card_shown_clause;
			card_shown_clause.push_back(GetPairNum(refuter, card_shown));
			solver->AddClause(card_shown_clause); 
		}
	}
}
void ClueReasoner::Accuse(string suggester, string card1, string card2, string card3, bool is_correct)
{
	// TO BE IMPLEMENTED AS AN EXERCISE
}
