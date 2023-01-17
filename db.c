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

    fprintf(stdout, "%lu\n", (unsigned long)time(NULL));

    conn = mysql_init(NULL);

    /* Connect to the database */
    // Bad practice, hard coded credentials. Just for POC
    if (!mysql_real_connect(conn, "localhost", "user", "password", "db", 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
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
        exit(1);
    }

    printf("Record inserted successfully.\n");

    /* Close the connection */
    mysql_close(conn);

    free(query);

    return 0;
}
