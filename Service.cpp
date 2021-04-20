#include "Repo.h"
#include "Expense.h"
#include "Service.h"
#include <time.h>
#include <bits/stdc++.h>
#include <cstring>

using namespace std;

void Service::clearStack(){
    while(!this->undoStack.empty())
        this->undoStack.pop();

}

Service::Service(){
    this->clearStack();
}

Service::Service(Repo& repo){
    this->repo = repo;
    this->clearStack();
}


Service::~Service(){
    this->clearStack();
}

int Service::get_size_repo() {
	return this->repo.get_size();
}


Expense* Service::get_all(){
    this->repo.get_all();
}


void Service::add_expense(int id, int day, int sum, char* type) {
    undoStack.push(std::make_pair(this->repo.get_all(), this->repo.get_size()));
	Expense e(id, day, sum, type);
	this->repo.add_expense(e);
	//add_to_undo_stack();
}

void Service::update_expense(int id, int new_day, int new_sum, char* new_type) {
    undoStack.push(std::make_pair(this->repo.get_all(), this->repo.get_size()));
    Expense e(id, new_day, new_sum, new_type);
	this->repo.update_expense(id, e);
	//add_to_undo_stack();
}


void Service::delete_expense(int id) {
    undoStack.push(std::make_pair(this->repo.get_all(), this->repo.get_size()));
	this->repo.delete_expense(id);
	//add_to_undo_stack();
}

void Service::add_expense_on_current_day(int id, int sum, char* type){
    undoStack.push(std::make_pair(this->repo.get_all(), this->repo.get_size()));
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);
    int day = aTime->tm_mday;
    Expense e(id, day, sum, type);
	this->repo.add_expense(e);
	//add_to_undo_stack();
}

void Service::delete_expense_by_day(int day){
    undoStack.push(std::make_pair(this->repo.get_all(), this->repo.get_size()));
    int i = 0;
    while(i < this->repo.get_size())
        if(this->repo.get_all()[i].get_day() == day)
            this->repo.delete_expense(this->repo.get_all()[i].get_id());
        else
            i++;
    //add_to_undo_stack();
}

void Service::delete_expense_by_type(char* type){
    undoStack.push(std::make_pair(this->repo.get_all(), this->repo.get_size()));
    int i = 0;
    while(i < this->repo.get_size())
            if(strcmp(this->repo.get_all()[i].get_type(), type) == 0)
                this->repo.delete_expense(this->repo.get_all()[i].get_id());
            else
                i++;
    //add_to_undo_stack();
}

void Service::delete_expense_within_range_days(int day_start, int day_end){
    undoStack.push(std::make_pair(this->repo.get_all(), this->repo.get_size()));
    int i = 0;
    while(i < this->repo.get_size()){
            int day = this->repo.get_all()[i].get_day();
            if(day >= day_start && day <= day_end)
                this->repo.delete_expense(this->repo.get_all()[i].get_id());
            else
                i++;
    }
    //add_to_undo_stack();
}

Expense* Service::get_expenses_by_type(char* type){
    Expense expenses[20];
    int n = 0;
    for(int i = 0; i < this->repo.get_size(); i++ )
        if (strcmp(this->repo.get_all()[i].get_type(), type) == 0){
            expenses[n] = this->repo.get_all()[i];
            n++;
        }
    return expenses;
}

Expense* Service::get_expenses_by_type_bigger_than_sum(char* type, int sum){
    Expense expenses[20];
    int n = 0;
    for(int i = 0; i < this->repo.get_size(); i++ )
        if (strcmp(this->repo.get_all()[i].get_type(), type) == 0 && this->repo.get_all()[i].get_sum() >= sum){
            expenses[n] = this->repo.get_all()[i];
            n++;
        }
    return expenses;
}

Expense* Service::get_expenses_by_type_equal_to_sum(char* type, int sum){
    Expense expenses[20];
    int n = 0;
    for(int i = 0; i < this->repo.get_size(); i++ )
        if (strcmp(this->repo.get_all()[i].get_type(), type) == 0 && this->repo.get_all()[i].get_sum() == sum){
            expenses[n] = this->repo.get_all()[i];
            n++;
        }
    return expenses;
}

int Service::get_total_sum_by_type(char* type){
    int sum = 0;
    for(int i = 0; i < this->repo.get_size(); i++ )
        if (strcmp(this->repo.get_all()[i].get_type(), type) == 0)
            sum = sum + this->repo.get_all()[i].get_sum();
    return sum;
}

int Service::get_most_expensive_day(){
    int days[32] = {0};
    for(int i = 0; i < this->repo.get_size(); i++ )
        days[this->repo.get_all()[i].get_day()] += this->repo.get_all()[i].get_sum();
    int mx = 0, day = 0;
    for (int i = 1; i < 32; i++ )
        if(days[i] >= mx){
            mx = days[i];
            day = i;
        }
    return day;
}


void Service::get_cost_in_descending_order_by_days(int days[]){
    for(int i = 0; i < this->repo.get_size(); i++ )
        days[this->repo.get_all()[i].get_day()] += this->repo.get_all()[i].get_sum();
    for(int i = 1; i < 31; i++)
        for(int j = i + 1; j < 32; j++)
            if(days[i] < days[j]){
                int aux = days[i];
                days[i] = days[j];
                days[j] = aux;
            }
}

void Service::get_cost_in_ascending_order_by_type(char* type, int days[]){
    for(int i = 0; i < this->repo.get_size(); i++ )
        if(strcmp(this->repo.get_all()[i].get_type(), type) == 0)
            days[this->repo.get_all()[i].get_day()] += this->repo.get_all()[i].get_sum();
    for(int i = 1; i < 31; i++)
        for(int j = i + 1; j < 32; j++)
            if(days[i] > days[j]){
                int aux = days[i];
                days[i] = days[j];
                days[j] = aux;
            }
}

void Service::filter_expenses_by_type(char* type){
    undoStack.push(std::make_pair(this->repo.get_all(), this->repo.get_size()));
    int i = 0;
    while(i < this->repo.get_size())
        if (strcmp(this->repo.get_all()[i].get_type(), type) != 0)
            this->repo.delete_expense(this->repo.get_all()[i].get_id());
        else
            i++;
    //add_to_undo_stack();
}


void Service::filter_expenses_by_type_smaller_than_sum(char* type, int sum){
    undoStack.push(std::make_pair(this->repo.get_all(), this->repo.get_size()));
    int i = 0;
    while(i < this->repo.get_size())
        if (strcmp(this->repo.get_all()[i].get_type(), type) != 0 || this->repo.get_all()[i].get_sum() > sum)
            this->repo.delete_expense(this->repo.get_all()[i].get_id());
        else
            i++;
    //add_to_undo_stack();
}


void Service::filter_expenses_by_type_equal_to_sum(char* type, int sum){
    undoStack.push(std::make_pair(this->repo.get_all(), this->repo.get_size()));
    int i = 0;
    while(i < this->repo.get_size())
        if (strcmp(this->repo.get_all()[i].get_type(), type) != 0 || this->repo.get_all()[i].get_sum() != sum)
            this->repo.delete_expense(this->repo.get_all()[i].get_id());
        else
            i++;
    //add_to_undo_stack();
}


bool Service::do_undo(){
    if (!undoStack.empty())
    {
        this->repo.set_vector(undoStack.top().first, undoStack.top().second);
        undoStack.pop();
        return true;
    }
    return false;
}



