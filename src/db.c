#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <string.h>
#include "parse.h"
#include "db.h"

#define DATABASE "db"
#define USERNAME "user"
#define PASSWORD "password"
#define TABLE "ecg"

static MYSQL *conn;

int dbInit(void) {
	// MYSQL_RES *res;
	// MYSQL_ROW row;

	conn = mysql_init(NULL);
	if(conn == NULL) {
		fprintf(stderr, "Can't initialize mySQL\n");
		return -1;
	}

	/* Connect to the database */
	// Bad practice, hard coded credentials. Just for POC
	if ( mysql_real_connect(conn, "127.0.0.1", USERNAME, PASSWORD, DATABASE, 0, NULL, 0) == NULL) {
		fprintf(stderr, "Can't connect to database: %s\n", mysql_error(conn));
		return -1;
	}

	return 0;
}

void dbClose(void) {
	/* Close the connection */
	mysql_close(conn);
}

int dbStore(struct dataSample_t *sample, uint64_t ECGID, int n) {
	char query[200];

	int i;
	for (i = 0; i < n; i++) {
		/* Form the query */
		sprintf(query, "INSERT INTO %s (ECGID, value, Timestamp) VALUES (%lu, %u, %lu)",
				TABLE, ECGID, sample[i].data, sample[i].timeStamp);

		/* Execute the query */
		if (mysql_query(conn, query)) {
			fprintf(stderr, "Can't execute query: %s\n", mysql_error(conn));
			return -1;;
		}
	}

	return 0;
}

// int main() {
// 	MYSQL *conn;
// 	MYSQL_RES *res;
// 	MYSQL_ROW row;
// 	char query[200];

// 	conn = mysql_init(NULL);
// 	if(conn == NULL) {
// 		fprintf(stderr, "Can't initialize mySQL\n");
// 		return -1;
// 	}

// 	/* Connect to the database */
// 	// Bad practice, hard coded credentials. Just for POC
// 	if ( mysql_real_connect(conn, "127.0.0.1", "user", "password", "db", 0, NULL, 0) == NULL) {
// 		fprintf(stderr, "Can't connect to database: %s\n", mysql_error(conn));
// 		exit(1);
// 	}

// 	/* Form the query */
// 	// Bad Practice, having hard coded values. Just for POC
// 	strcpy(query, "INSERT INTO ecg (ECGID, value, Timestamp) VALUES (1, 27, 1673958673)");

// 	/* Execute the query */
// 	if (mysql_query(conn, query)) {
// 		fprintf(stderr, "Can't execute query: %s\n", mysql_error(conn));
// 		exit(1);
// 	}

// 	/* Close the connection */
// 	mysql_close(conn);

// 	printf("Success\n");

// 	return 0;
// }
