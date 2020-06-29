#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

MYSQL* connection = NULL;
MYSQL conn;
MYSQL_RES* sql_result;
MYSQL_ROW sql_row;
const char* host = "localhost";
const char* user = "root";
const char* pw = "thsldk0520";
const char* db = "delivery_system";
char drop_sql[10][1000] = { 0, };

void create_DB();
void selectQueryType();
void type1();
void subType1();
void subType2();
void subType3();
void type2();
void type3();
void type4();
void type5();
void drop_DB();


void create_DB() {
	FILE* fp = fopen("20171666.txt", "r");
	char query[1000];
	int state = 0;
	int i = 0;

	while (fgets(query, sizeof(query), fp) != NULL) {
		//printf("%s", query);
		if (query[0] == 'd') {
			strcpy(drop_sql[i], query);
			i++;
		}
		else if (query[0] == 'c' || query[0] == 'i') {
			state = mysql_query(connection, query);
		}
	}

	fclose(fp);
} // create table, insert data and save drop sql queries.

void selectQueryType() {
	int user_input;

	while (1) {
		printf("------- SELECT QUERY TYPES -------\n\n");
		printf("\t1. TYPE I\n");
		printf("\t2. TYPE II\n");
		printf("\t3. TYPE III\n");
		printf("\t4. TYPE IV\n");
		printf("\t5. TYPE V\n");
		printf("\t0. QUIT\n");
		printf("Which type of query? ");

		scanf("%d", &user_input);

		switch (user_input) {
		case 1:
			type1();
			break;
		case 2:
			type2();
			break;
		case 3:
			type3();
			break;
		case 4:
			type4();
			break;
		case 5:
			type5();
			break;
		case 0:
			printf("Good Bye! :)\n");
			return;
		}
	}
} // main query menu. loop until input 0

void type1() {
	int user_input;
	int truck_num;

	printf("---- TYPE I ----\n");
	printf("** Truck 1721 is destroyed in a crash at 2020-04-13\n\n");
	// crushed at my certain time == 2020-04-13

	while (1) {
		printf("---- Subtypes in TYPE I ----\n");
		printf("\t1. TYPE I-1\n");
		printf("\t2. TYPE I-2\n");
		printf("\t3. TYPE I-3\n");
		printf("Which type of query? ");

		scanf("%d", &user_input);

		switch (user_input) {
		case 1:
			subType1();
			break;
		case 2:
			subType2();
			break;
		case 3:
			subType3();
			break;
		case 0:
			printf("\n\n");
			return;
		}
	}
} // menu type 1. 

void subType1() {
	char query[1000];
	int state;

	printf("---- TYPE I-1 ----\n");
	printf("** Find all Find all customers who had a package on the truck at the time of the crash. **\n");
	//sql and print result
	strcpy(query,"select distinct customer.name from customer, decomposed_history, history \
	where customer.customer_ID = decomposed_history.customer_ID and decomposed_history.package_ID = history.package_ID \
		and history.vehicle = \'truck1721\' and date(history.history_time) = \'2020-04-13\'");

	state = mysql_query(connection, query);
	if (state == 0)
	{
		printf("Customer name: ");
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("%s ", sql_row[0]);
		}
		mysql_free_result(sql_result);
	}
	printf("\n\n");
	return;
} // type 1-1. 

void subType2() {
	int state;
	char query[1000];

	printf("---- TYPE I-2 ----\n");
	printf("** Find all recipients who had a package on that truck at the time of the crash. **\n");

	strcpy(query, "select distinct package.recipient_name from package, decomposed_history, history \
	where package.package_ID = decomposed_history.package_ID and decomposed_history.package_ID = history.package_ID\
		and history.vehicle = \'truck1721\' and date(history.history_time) = \'2020-04-13\'");

	state = mysql_query(connection, query);
	if (state == 0)
	{
		printf("Recipient name: ");
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("%s ", sql_row[0]);
		}
		mysql_free_result(sql_result);
	}
	printf("\n\n");
	return;
} // type 1-2

void subType3() {
	char query[1000];
	int state;

	printf("---- TYPE I-3 ----\n");
	printf("** Find the last successful delivery by that truck prior to the crash. **\n");

	strcpy(query, "select decomposed_history.package_ID, decomposed_history.customer_ID from decomposed_history, history\
	where  decomposed_history.package_ID = history.package_ID\
		and\
		history.history_time = (select max(history.history_time) from history, ship\
	where history.package_ID = ship.package_ID and\
		ship.complete = \'complete\' and\
		history.vehicle = \'truck1721\' and\
		date(history.history_time) < '2020-04-13'); ");

	state = mysql_query(connection, query);
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("Package ID: %s ", sql_row[0]);
			printf("Customer ID: %s ", sql_row[1]);
		}
		mysql_free_result(sql_result);
	}
	printf("\n\n");
} // type 1-3

void type2() {
	char user_input[5];
	int state;
	char query[1000];

	printf("---- TYPE II ----\n");

	while (1) {
		printf("** Find the customer who has shipped the most packages in the past certain year. **\n");
		printf("Which year? ");

		scanf("%s", &user_input);
		if (strcmp(user_input,"0") == 0) {
			printf("\n");
			return;
		} // get user input
		//sql
		strcpy(query, "with countship(customer_ID, ship_count) AS \
		(select ship.customer_ID, count(*) as ship_count\
			from ship, package\
		where ship.package_ID = package.package_ID\
		and year(package.promised_time) = \'");
		strcat(query, user_input);
		strcat(query, "\'\
			group by ship.customer_ID)\
			select DISTINCT ship.customer_ID from ship, countship\
		where countship.ship_count = (select max(countship.ship_count) from countship)\
			and countship.customer_ID = ship.customer_ID");

		state = mysql_query(connection, query);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				printf("Customer ID: %s ", sql_row[0]);
			}
			mysql_free_result(sql_result);
		}
		
		printf("\n\n");
	}

} // type 2. 

void type3() {
	char user_input[5];
	int state;
	char query[1000];

	printf("---- TYPE III ----\n");

	while (1) {
		printf("** Find the customer who has spent the most money on shipping in the past certain year.**\n");
		printf("Which year? ");

		scanf("%s", &user_input);
		if (strcmp(user_input, "0") == 0) {
			printf("\n");
			return;
		} // get user input
		//sql
		strcpy(query, "with payment(customer_ID, total_pay) AS \
		(select bill.customer_ID, sum(bill.cost) as total_pay\
			from bill\
		where year(bill.bill_time) = \'");
		strcat(query, user_input);
		strcat(query, "\'\
			group by bill.customer_ID)\
			select DISTINCT bill.customer_ID from bill, payment\
		where payment.total_pay = (select max(payment.total_pay) from payment)\
			and bill.customer_ID = payment.customer_ID");

		state = mysql_query(connection, query);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				printf("Customer ID: %s ", sql_row[0]);
			}
			mysql_free_result(sql_result);
		}
		printf("\n\n");
	}
} // type 3.

void type4() {
	char user_input[5];
	int state;
	char query[1000];

	printf("---- TYPE IV ----\n");
	printf("** Find those packages that were not delivered within the promised time **\n");

	//sql
	strcpy(query, "select package.package_ID from package, history\
	where package.package_ID = history.package_ID\
	and date(package.promised_time) < date(history.history_time)\
		group by package.package_ID");

	state = mysql_query(connection, query);
	if (state == 0)
	{
		printf("Package ID: ");
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("%s ", sql_row[0]);
		}
		mysql_free_result(sql_result);
	}
	printf("\n\n");

	return;
} // type 4

void type5() {
	char id[20];
	char year[10];
	char month[10];
	int state;
	char query[1000];
	FILE* outf = fopen("bill.txt", "w");

	getchar();
	printf("---- TYPE V ----\n");
	printf("** Generate the bill for each customer for the past certain month. **\n");
	printf("Customer ID : ");
	scanf("%[^\n]s", id);
	getchar();
	printf("Which month(YYYY-MM)? ");
	scanf("%[^\n]s", year);
	strcpy(month, year+5); // get user input and divided into year and month
	year[4] = '\0';
	//sql

	strcpy(query,"select customer.name, customer.city, customer.street, sum(bill.cost)\
		from bill, customer\
	where customer.customer_ID = \'");
	strcat(query, id);
	strcat(query, "\'\
		and customer.customer_ID = bill.customer_ID\
		and year(bill.bill_time) = \'"); 
	strcat(query, year);
	strcat(query, "\'\
		and month(bill.bill_time) = \'");
	strcat(query, month);
	strcat(query, "\'\
		group by customer.customer_ID");

	state = mysql_query(connection, query);
	if (state == 0)
	{
		fprintf(outf, "Customer name\tAddress-city\tstreet\tAmount\n");
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			fprintf(outf,"%s\t\t%s\t\t%s\t\t%s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
		}
		mysql_free_result(sql_result);
	}
	fprintf(outf,"\n"); // print simple bill

	strcpy(query, "select customer.name, bill.pay_type, customer.city, customer.street, sum(bill.cost)\
		from bill, customer\
	where customer.customer_ID = \'");
	strcat(query, id);
	strcat(query, "\'\
		and customer.customer_ID = bill.customer_ID\
		and year(bill.bill_time) = \'");
	strcat(query, year);
	strcat(query, "\'\
		and month(bill.bill_time) = \'");
	strcat(query, month);
	strcat(query, "\'\
		group by bill.pay_type");
	state = mysql_query(connection, query);
	if (state == 0)
	{
		fprintf(outf, "======================================================================\n");
		fprintf(outf, "Customer name\tPay type\tAddress-city\tstreet\tAmount\n");
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			fprintf(outf, "%s\t\t%s\t\t%s\t\t%s\t\t%s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4]);
		}
		mysql_free_result(sql_result);
	}
	fprintf(outf, "\n"); // print bill with pay type

	strcpy(query, "select package.package_ID, bill.cost, package.package_type, bill.pay_type, ship.complete\
		from bill, package, ship\
	where ship.customer_ID= \'");
	strcat(query, id);
	strcat(query, "\'\
		and ship.customer_ID=bill.customer_ID\
		and year(bill.bill_time) = \'");
	strcat(query, year);
	strcat(query, "\'\
		and month(bill.bill_time) = \'");
	strcat(query, month);
	strcat(query, "\'\
		and package.package_ID=bill.package_ID\
		and package.package_ID = ship.package_ID");

	state = mysql_query(connection, query);
	if (state == 0)
	{
		fprintf(outf, "======================================================================\n");
		fprintf(outf, "Package ID\tCost\tPackage type\tPay type\tDelivery complete\n");
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			fprintf(outf, "%s\t\t%s\t\t%s\t\t%s\t\t%s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4]);
		}
		mysql_free_result(sql_result);
	}
	fprintf(outf, "\n"); // print itemize billing

	fclose(outf);
	printf("Generation Completed\n\n");

	return;
} // type 5

void drop_DB() {
	int i = 0;
	int state = 0;

	while (1) {
		if (drop_sql[i][0] == '0')
			break;
		//printf("%s", drop_sql[i]);
		state = mysql_query(connection, drop_sql[i]);
		i++;
	}
} // drop the tables I make

int main(void) {
	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL)
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	} // connect to database

	create_DB();

	printf("***********************************************\n");
	printf("** Sogang Package Delivery Management System **\n");
	printf("***********************************************\n\n\n");

	selectQueryType();
	drop_DB();

	mysql_close(connection);
	return 0;
}