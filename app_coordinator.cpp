#include "UI.h"
#include "Service.h"
#include "Repo.h"
#include "Expense.h"

Repo repo;
Service service(repo);
Console console(service);

run_console();

