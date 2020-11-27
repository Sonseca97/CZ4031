import psycopg2
import json 
import pandas as pd 
import os

class DBConnection:
    def __init__(self, host="localhost", port = 5432, database="tpch", user="postgres", password="92685600"):
        self.conn = psycopg2.connect(host=host, port=port, database=database, user=user, password=password)
        self.cur = self.conn.cursor()
     

    def execute(self, query, isTable=False):
        self.cur.execute(query)
        cols = None
        if isTable:
            cols = [desc[0] for desc in self.cur.description]
        query_results = self.cur.fetchall()
        return query_results, cols

    def close(self):
        self.cur.close()
        self.conn.close()
    
    def get_table(self, table_name):
        query = 'SELECT * FROM ' + table_name
        result, cols = self.execute(query, isTable=True)
        df = pd.DataFrame(result, columns=cols)
        return df


if __name__ == "__main__":
    # connection = DBConnection()
    # query = 'SELECT * FROM picassoplantree'
    # connection = DBConnection()
    # result, cols = connection.execute(query)
    # df = pd.DataFrame(result, columns=cols)
    # connection.close()
    path = 'C:\Program Files (x86)\picasso2.1\picasso2.1\PicassoRun\Windows'
    server = 'runServer.bat'
    os.chdir(path)
    os.startfile(server)


