/**
 *
 * Base SINGLE thread example of using MySQL Connector Wrapper   
 *
 */

#include "fl/db/Database.hpp"
using namespace Forward;

#define ADDRESS	"tcp://127.0.0.1:3306"		
#define USER_NAME "root"
#define USER_PASSWORD "testing"

#define DB_NAME "test"

int main(int, char*)
{
	// Making scope object to destroy it after method exit
	auto db = Database::InitScoped();

	// Connecting to MySQL database using address, user name and password
	bool is_conn = db->Connect(ADDRESS, USER_NAME, USER_PASSWORD);

	// Check if connection succeded 

	if (!is_conn)
	{
		return EXIT_FAILURE;
	}

	// Setting database schema

	db->SetActiveSchema(DB_NAME);

	// Checking if schema was set

	if (!db->IsActiveSchema())
	{
		return EXIT_FAILURE;
	}

	// Executing queries
	// Keep in mind that return type of executed query is SCOPED pointer

	{
		// Getting current time from database
		// Make sure to provide correct variable name like 'current_time' or using index of a row
		// Also you can use prepared queries using '?' as value that will be inserted

		auto result = db->Execute("SELECT NOW()");

		// Checking if result is valid

		if (!result)
		{
			return EXIT_FAILURE;
		}

		// Checking if result has first row

		if (!result->first())
		{
			return EXIT_FAILURE;
		}

		// Getting column from row with index '1'

		auto time = result->getString(1);

		std::cout << "Current DB time is " << time;
	}

	return EXIT_SUCCESS;
}