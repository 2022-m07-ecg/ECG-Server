#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <time.h>
#include <string.h>

int main() {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char *query;
	char timestamp[20];
	time_t now;

	query = malloc(1000);

	printf("%lu\n", (unsigned long)time(NULL));

	conn = mysql_init(NULL);
	if(conn == NULL) {
		fprintf(stderr, "Can't initialize mySQL\n");
		free(query);
		return -1;
	}

	/* Connect to the database */
	// Bad practice, hard coded credentials. Just for POC
	if ( mysql_real_connect(conn, "127.0.0.1", "user", "password", "db", 0, NULL, 0) == NULL) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		free(query);
		exit(1);
	}

	/* Get the current timestamp */
	now = time(NULL);
	sprintf(timestamp, "%ld", (long) now);

	/* Form the query */
	// Bad Practice, having hard coded values. Just for POC
	query = "INSERT INTO ecg (ECGID, value, Timestamp) VALUES (1, 27, 1673958673)";

	/* Execute the query */
	if (mysql_query(conn, query)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		free(query);
		exit(1);
	}

	/* Close the connection */
	mysql_close(conn);

	// Segfaults for some reason
	free(query);

	printf("Success\n");

	return 0;
}
