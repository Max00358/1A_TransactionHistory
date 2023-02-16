#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

//g++ -o main History_Transaction_definitions.cpp project4.cpp main.cpp -std=c++11
//g++ -o main History_Transaction_definitions main.cpp project4.cpp History_Transaction_definitions.cpp -std=c++11

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////

// Constructor
// TASK 1
//
Transaction::Transaction (std::string ticker_symbol, 
							unsigned int day_date, 
							unsigned int month_date, 
							unsigned int year_date, 
							bool buy_sell_trans, 
							unsigned int number_shares, 
							double trans_amount){
	
	p_next = nullptr;

	acb = 0.0;
	acb_per_share = 0.0;
  	share_balance = 0.0;
  	cgl = 0.0;

	symbol = ticker_symbol;
	day = day_date;
	month = month_date;
	year = year_date;
	if(buy_sell_trans == true){
		trans_type = "Buy";
	}
	else
		trans_type = "Sell";
	
	shares = number_shares;
	amount = trans_amount;
	trans_id = assigned_trans_id;

	++assigned_trans_id;
}

// Destructor
// TASK 1
//
Transaction::~Transaction(){

}

// Overloaded < operator.
// TASK 2
//
bool Transaction::operator<(Transaction const &other){
	//'this' refers to the left operand (or use the member variable directly) e.g. symbol
	//'other' refers to the right operand e.g. other.symbol

	//Have full access to private
	//Compare year, months and date
	
	if(this->year < other.year){ // or year < other.year
		return true;
	}
	else if(this->year == other.year && this->month < other.month){
		return true;
	}
	else if(this->year == other.year && this->month == other.month && this->day < other.day){
		return true;
	}

	return false;
}

// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
	std::cout << std::fixed << std::setprecision(2);
	std::cout << std::setw(4) << get_trans_id() << " "
		<< std::setw(4) << get_symbol() << " "
		<< std::setw(4) << get_day() << " "
		<< std::setw(4) << get_month() << " "
		<< std::setw(4) << get_year() << " ";


	if ( get_trans_type() ) {
		std::cout << "  Buy  ";
	} else { std::cout << "  Sell "; }

	std::cout << std::setw(4) << get_shares() << " "
		<< std::setw(10) << get_amount() << " "
		<< std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
		<< std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
		<< std::setw(10) << std::setprecision(3) << get_cgl()
		<< std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////

// Constructor
// TASK 3
//
History::History(){
	this->p_head = nullptr;
}

// Destructor
// TASK 3
//
History::~History(){
	//Code developed in David's Lab session
	Transaction *p_temp = p_head;

	while(p_head != nullptr){
		p_temp = p_temp -> get_next();
		delete p_head;
		p_head = p_temp;
	}
}

// read_history(...): Read the transaction history from file.
// TASK 4
//
void History::read_history(){
	ece150::open_file();

	while(ece150::next_trans_entry()){
		Transaction *new_trans = new Transaction(ece150::get_trans_symbol(),
												ece150::get_trans_day(), 
												ece150::get_trans_month(), 
												ece150::get_trans_year(), 
												ece150::get_trans_type(), 
												ece150::get_trans_shares(), 
												ece150::get_trans_amount() );
		History::insert(new_trans);
	}
	ece150::close_file();
}
// insert(...): Insert transaction into linked list.
// TASK 5
//
void History::insert(Transaction * p_new_trans){
	//Code developed in ECE 150 lab session w/ David Lau
	if(p_head == nullptr){
		this->p_head = p_new_trans;
	}
	else{
		Transaction *p_find = p_head;

		while(p_find->get_next() != nullptr){
			p_find = p_find->get_next();
		}
		p_find->set_next(p_new_trans);
	}
}

// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//
//g++ -o main History_Transaction_definitions.cpp project4.cpp main.cpp -std=c++11
void History::sort_by_date(){
	Transaction *p_front = nullptr;
	Transaction *p_head_sort = nullptr;

	while(p_head != nullptr){
		//isolating front node
		p_front = p_head;
		p_head = p_head -> get_next();
		p_front -> set_next(nullptr);

		//inserting node into sorted list
		if(p_head_sort == nullptr){
			p_head_sort = p_front;
			p_front = nullptr;
		}
		else{
			//Insert at the very front
			
			if(*p_front < *p_head_sort && p_front != nullptr){
				p_front -> set_next(p_head_sort);
				p_head_sort = p_front;
				p_front = nullptr;
			}
			
			else{
				Transaction *p_traverse = p_head_sort;
				while( (p_traverse -> get_next() != nullptr) && (*(p_traverse -> get_next()) < *p_front) ){//Compare value instead of memory address
					p_traverse = p_traverse -> get_next();
				}
				//When the date is the same
				
				if( (p_traverse -> get_year() == p_front -> get_year()) && 
					(p_traverse -> get_month() == p_front -> get_month()) && 
					(p_traverse -> get_day() == p_front -> get_day()) ){
					
					if( (p_traverse -> get_trans_id()) > (p_front -> get_trans_id()) ){//Compare trans ID
						Transaction *p_traverse_traverse = p_head_sort;
						while(*(p_traverse_traverse -> get_next()) < *p_traverse){//Compare value instead of memory address
							p_traverse_traverse = p_traverse_traverse -> get_next();
						}
						p_front -> set_next(p_traverse);
						p_traverse_traverse -> set_next(p_front);
						p_front = nullptr;
						p_traverse = nullptr;
						p_traverse_traverse = nullptr;
					}
					else{
						//Do the insert
						p_front -> set_next(p_traverse -> get_next());
						p_traverse -> set_next(p_front);
						p_front = nullptr;
						p_traverse = nullptr;
					}
				}
				
				else{
					//Do the insert
					p_front -> set_next(p_traverse -> get_next());
					p_traverse -> set_next(p_front);
					p_front = nullptr;
					p_traverse = nullptr;
				}
			}
		}
	}
	p_head = p_head_sort;
	p_head_sort = nullptr;
}

// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//
//g++ -o main History_Transaction_definitions.cpp project4.cpp main.cpp -std=c++11
void History::update_acb_cgl(){
	Transaction *p_head_new = p_head;
	double now_acb = 0.0;
	double next_acb = 0.0;
	double tot_acb = 0.0;

	double now_balance = 0.0;
	double next_balance = 0.0;
	double tot_balance = 0.0;

	double ACB_Share = 0.0;
	double cgl = 0.0;

	while(p_head_new != nullptr){
		if(p_head_new -> get_trans_type() == true){
			//alter acb
			now_acb = p_head_new -> get_amount();
			tot_acb += now_acb;
			p_head_new -> set_acb(tot_acb);

			//alter share balance
			now_balance = p_head_new -> get_shares();
			tot_balance += now_balance;
			p_head_new -> set_share_balance(tot_balance);

			//alter ACB/Share
			ACB_Share = tot_acb / tot_balance;
			p_head_new -> set_acb_per_share(ACB_Share);

			p_head_new = p_head_new -> get_next();
			
		}
		else{
			//alter ACB/Share
			p_head_new -> set_acb_per_share(ACB_Share);
			double sell_a_s = p_head_new -> get_acb_per_share();

			//alter acb
			double shares = p_head_new -> get_shares();
			tot_acb = tot_acb - (sell_a_s * shares);
			p_head_new -> set_acb(tot_acb);

			//alter share balance
			double minus_share_balance = p_head_new -> get_shares();
			tot_balance -= minus_share_balance;
			p_head_new -> set_share_balance(tot_balance);

			//calc cgl
			double amount_paid = p_head_new -> get_amount();
			cgl = amount_paid - (shares * sell_a_s);
			p_head_new -> set_cgl(cgl);

			p_head_new = p_head_new -> get_next();
		}
	}
}

// compute_cgl(): Compute the ACB, and CGL.
// TASK 8
double History::compute_cgl(unsigned int year){
	Transaction *p_new_head = p_head;
	double tot_cgl = 0;

	while(p_new_head != nullptr){
		if(p_new_head -> get_year() == year){
			tot_cgl += p_new_head -> get_cgl();
		}
		p_new_head = p_new_head -> get_next();
	}

	return tot_cgl;
}

// print() Print the transaction history.
//TASK 9
//
//g++ -o main History_Transaction_definitions.cpp project4.cpp main.cpp -std=c++11
void History::print(){

	Transaction *p_new = p_head;
	std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;
	while(p_new != nullptr){
		p_new->print();
		p_new = p_new->get_next();
	}
	std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;
	
}

// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
