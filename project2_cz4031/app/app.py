import sys
import json
import tkinter as tk
import os 
from tkinter import *
from tkinter.font import Font
from tkinter import messagebox
from query_description import *
from pyconnect import DBConnection
import argparse


#python app/app.py --host localhost --port 5432 --database tpch --user postgres --password 92685600

class App(object):

    def __init__(self, parent, args):
        self.root = parent
        self.root.title("Query Explanation")
        self.frm_input_text = tk.Frame(self.root)
        self.frm_input_text.pack()
        self.frm_input = tk.Frame(self.root)
        self.frm_input.pack()
        self.frm_line = tk.Frame(self.root)
        self.frm_line.pack()
        canvas = Canvas(self.frm_line, width=2000, height=20)
        canvas.create_line(0, 15, 2000, 15)
        canvas.pack()
        output_font = Font(family=None, size=15)
        self.frm_nlp_text = tk.Frame(self.root)
        self.frm_nlp_text.pack()
        self.frm_nlp = tk.Frame(self.root)
        self.frm_nlp.pack()
        self.frm_tree_text = tk.Frame(self.root)
        self.frm_tree_text.pack()
        self.frm_tree = tk.Frame(self.root)
        self.frm_tree.pack()
        self.frm_diff_text = tk.Frame(self.root)
        self.frm_diff_text.pack()
        self.frm_diff = tk.Frame(self.root)
        self.frm_diff.pack()

        self.frm_input_t = tk.Frame(self.frm_input)
        self.frm_input_t.pack(side=LEFT)
        self.frm_input_btt = tk.Frame(self.frm_input)
        self.frm_input_btt.pack(side=RIGHT)

        self.frm_nlp_t = tk.Frame(self.frm_nlp)
        self.frm_nlp_t.pack(side=LEFT)
        self.frm_nlp_btt = tk.Frame(self.frm_nlp)
        self.frm_nlp_btt.pack(side=RIGHT)

        self.frm_tree_t = tk.Frame(self.frm_tree)
        self.frm_tree_t.pack(side=LEFT)
        self.frm_tree_btt = tk.Frame(self.frm_tree)
        self.frm_tree_btt.pack(side=RIGHT)

        self.frm_tree_t = tk.Frame(self.frm_tree)
        self.frm_tree_t.pack(side=LEFT)
        self.frm_tree_btt = tk.Frame(self.frm_tree)
        self.frm_tree_btt.pack(side=RIGHT)

        self.frm_diff_t = tk.Frame(self.frm_diff)
        self.frm_diff_t.pack(side=LEFT)
        self.frm_diff_btt = tk.Frame(self.frm_diff)
        self.frm_diff_btt.pack(side=RIGHT)

        self.input_text1 = tk.Label(
            self.frm_input_text, text='Please Input Query:', font=(None, 16), width=60)
        self.input_text1.pack(side=LEFT, pady=5)
        self.input_text2 = tk.Label(
            self.frm_input_text, text='Please input lines from sql code to replace, followed by\n replaced code in picasso templates form seperated by comma. ', font=(None, 12), width=75)
        self.input_text2.pack(side=RIGHT, pady=5)

        self.input1 = tk.Text(self.frm_input_t, relief=GROOVE,
                              width=75, height=8, borderwidth=5, font=(None, 12))
        self.input1.pack(side=LEFT, padx=10)
        self.input2 = tk.Text(self.frm_input_t, relief=RIDGE,
                              width=60, height=4, borderwidth=5, font=(None, 12))
        self.input2.pack(side=RIGHT, padx=10)

        self.view = tk.Button(self.frm_input_btt, text="view output",
                              width=10, height=2, command=self.retrieve_input)
        self.view.pack(pady=10)

        self.clear = tk.Button(self.frm_input_btt, text="clear input",
                               width=10, height=2, command=self.clear_input)
        self.clear.pack(pady=10)

        self.nlp_text1 = tk.Label(
            self.frm_nlp_text, text='Query Execution Plan:', font=(None, 16), width=60)
        self.nlp_text1.pack(side=LEFT, pady=5)
      

        self.nlp1 = tk.Text(self.frm_nlp_t, relief=GROOVE, width=75,
                            height=8, borderwidth=5, font=(None, 12), state='disabled')
        self.nlp1.pack(side=LEFT, padx=10)
        self.placeholder1 = tk.Label(self.frm_nlp_btt, width=10)
        self.placeholder1.pack()

        self.tree_text1 = tk.Label(
            self.frm_tree_text, text='Query Tree Structure:', font=(None, 16), width=60)
        self.tree_text1.pack(side=LEFT, pady=5)
    

        self.tree1 = tk.Text(self.frm_tree_t, relief=GROOVE, width=75,
                             height=8, borderwidth=5, font=(None, 12), state='disabled')
        self.tree1.pack(side=LEFT, padx=10)
        self.placeholder2 = tk.Label(self.frm_tree_btt, width=10)
        self.placeholder2.pack()

        self.placeholder3 = tk.Label(self.frm_diff_text, width=60)
        self.placeholder3.pack(pady=5)

        self.exp_text = tk.Label(
            self.frm_diff_text, text='Explanation for selecting this QEP by DBMS', font=(None, 16), width=60)
        self.exp_text.pack(side=LEFT, pady=5)

        self.exp = tk.Text(self.frm_diff_t, relief=GROOVE, width=155,
                            height=15, borderwidth=5, font=(None, 12), state='disabled')
        self.exp.pack(side=LEFT, padx=10)

        self.clear_out = tk.Button(
            self.frm_diff_btt, text="clear output", width=10, height=2, command=self.clear_output)
        self.clear_out.pack(pady=10)

        self.quit_ = tk.Button(self.frm_diff_btt, text="quit program",
                               width=10, height=2, command=self.quitprogram)
        self.quit_.pack(pady=10)

        self.psp = ''
        self.explanation_dict = {
            'Hash Join': 'it has best performance on large, sorted and non-indexed inputs, it is ideal for joining large tables.\n\n',
            'Nested Loop': 'it has the best performance on small inputs, it is the fastest on join due to having the least number of comparison.\n\n',
            'Index Scan': 'it performs much better when retrieving an insignificant number of rows compared to the total number rows.\n\n',
            'Seq Scan': 'it only requires a single i/o to fetch multiple rows from a block and is much faster than index scan.\n\n',
            'Merge Join': 'it has the best performance on sorted input, it is ideal for joining two independent inputs.\n\n'
        }
        self.host = args.host
        self.port = args.port
        self.database = args.database
        self.user = args.user
        self.password = args.password
        self.path = args.picassopath
        self.dbdescriptor = args.dbdescriptor
        self.server = 'runServer.bat'
        self.owd = os.getcwd()
        os.chdir(self.path)
        os.startfile(self.server)
        print("started picasso server")
        

    def retrieve_input(self):
        global query_old
        global query_new
        global desc
        global result
        query_old = self.input1.get("1.0", END)
        self.attributes = self.input2.get("1.0", END)
        self.query_picasso = self.convert_picasso(query_old)

        os.chdir(self.owd)
        # save the picasso template into sql file
        with open("picasso.sql", 'w') as file:
            file.write(self.query_picasso)
        file.close()
    
        sql_path = os.path.join(os.getcwd(), 'picasso.sql')
        sql_path = sql_path.replace('\\', '\\\\')

        #change to picasso directory
        os.chdir(self.path)

        cmd = "cmd /k PicassoCmd localhost 4444 " + self.dbdescriptor + " default POSTGRES_Default_U_ Uniform Compilation "+sql_path+' 10'

        print(cmd)
        os.system(cmd)
    
        print("send to picasso...")
        # change to original directory
        os.chdir(self.owd)

        result_old = self.get_query_result(query_old)
        result_old_obj = json.loads(json.dumps(result_old))
        result_old_nlp = self.get_description(result_old_obj)
        result_old_tree = self.get_tree(result_old_obj)

        result_picasso = self.get_picasso(query_old)

        self.nlp1.configure(state='normal')
        # self.nlp2.configure(state='normal')
        self.tree1.configure(state='normal')
        # self.tree2.configure(state='normal')
        self.exp.configure(state='normal')

        self.nlp1.delete("1.0", END)
        self.nlp1.insert(END, result_old_nlp)
        # self.nlp2.delete("1.0", END)
        # self.nlp2.insert(END, result_new_nlp)
        self.tree1.delete("1.0", END)
        self.tree1.insert(END, result_old_tree)
        # self.tree2.delete("1.0", END)
        # self.tree2.insert(END, result_new_tree)
        self.exp.delete("1.0", END)
        self.exp.insert(END, result_picasso)

    def convert_picasso(self, query):

        query_list = query.split()
        attributes = self.attributes.split(',')
        i = 0
        while i < len(attributes):
            self.psp += attributes[i]
            query = query.replace(attributes[i], attributes[i+1].strip())
            i += 2
        # whr_idx = query_list.index("where")

        # for attr in attributes:
        #     query_list.insert(whr_idx+1, attr)
        #     query_list.insert(whr_idx+2, ':varies')
        #     query_list.insert(whr_idx+3, 'and')
        #     whr_idx += 3
        # picasso_template = ' '.join(query_list)
        return query


    def clear_input(self):
        self.input1.delete("1.0", END)
        self.input2.delete("1.0", END)

    def clear_output(self):
        self.nlp1.delete("1.0", END)
        # self.nlp2.delete("1.0", END)
        self.tree1.delete("1.0", END)
        # self.tree2.delete("1.0", END)
        self.exp.delete("1.0", END)
        self.nlp1.configure(state='disabled')
        # self.nlp2.configure(state='disabled')
        self.tree1.configure(state='disabled')
        # self.tree2.configure(state='disabled')
        self.exp.configure(state='disabled')

    def get_query_result(self, query):
        # DBConnection takes 5 arguments
        connection = DBConnection(self.host, self.port, self.database, self.user, self.password)
        query_ex = 'explain (analyze, costs, verbose, buffers, format json) ' + query
        result, _ = connection.execute(query_ex)
        connection.close()
        self.sql_cost = result[0][0][0]['Plan']['Total Cost']
        return result[0][0]

    def get_description(self, json_obj):
        descriptions = get_text(json_obj)
        result = ""
        for description in descriptions:
            result = result + description + "\n"
        return result

    def get_tree(self, json_obj):
        head = parse_json(json_obj)
        return generate_tree("", head)
    
    def get_picasso(self, query):
        connection = DBConnection(self.host, self.port, self.database, self.user, self.password)
        result = ''
        df_plan_tree = connection.get_table('picassoplantree')
        # df_plan_tree.to_csv("plantree.csv", index=False)
        df_plan_store = connection.get_table('picassoplanstore')
        # df_plan_store.to_csv('planstore.csv', index=False)
        df_selectivitylog = connection.get_table('picassoselectivitylog')
        # df_selectivitylog.to_csv("selectivitylog.csv", index=False)


        # functions for plantree
        num_plan = len(df_plan_tree.planno.unique())
        # using cost to get postgresql plan
        plan_max_idx_by_range = df_plan_store[(df_plan_store.cost>(self.sql_cost-2000))&(df_plan_store.cost<(self.sql_cost+2000))].planno.value_counts().index.tolist()[0] 
        plan_max_by_range = df_plan_store.planno.value_counts().loc[plan_max_idx_by_range]

        # using gini to get postgresql plan
        # plan_count = df_plan_store.groupby('planno').count()
        # plan_max = plan_count['qtid'].max()
        # plan_max_idx = plan_count[plan_count.qtid==plan_max].index.values.tolist()[0]
        
        # calculate gini
        gini_plan = plan_max_by_range / 100
        pos_df = df_plan_tree[df_plan_tree.planno==plan_max_idx_by_range]
 
        attributes = self.psp.replace('and', '')
        result += "Picasso can generate {} plans by this setting.\n".format(num_plan)
        result += "Among these plans, plan number {} is the plan chosen by Postgresql with Gini coefficient {}. " \
            .format(plan_max_idx_by_range, gini_plan)
        result += "The query’s where condition {} will determine the selectivity space and thus determine the steps which Postgres take in order to compute the most optimal plan. "\
                    .format(attributes)
        result += "Compare to Picasso's {} plans, each plan would be the most optimal for a certain selectivity space. In this case, plan {} is the most optimal plan, with a Gini index of {}.\n\n"\
                    .format(num_plan, plan_max_idx_by_range, gini_plan)
        result += "---------Statistics and Analysis-----------\n"

        op_list = ['Nested Loop', 'Merge Join','Index Scan', 'Seq Scan', 'Hash Join']
        for i, op in pos_df.iterrows():
            if op['name'] not in op_list:
                continue 
            else:
                o_id = op['id']
                op_prev = ' '.join(pos_df[pos_df['parentid']==o_id]['name'].values.tolist())
                result += "-Operation of {} on/after {} with cost {} and cardinality {} after the operation. \n".format(op['name'], op_prev, op['cost'], op['card'])
                result += "-{} is used because ".format(op['name']) + self.explanation_dict[op['name']]
        return result
        
    def quitprogram(self):
        result = messagebox.askokcancel(
            "Quit the game.", "Are you sure?", icon='warning')
        if result == True:
            self.root.destroy()


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--host', help='postgresql connection host')
    parser.add_argument('--port', help='postgresql connection port')
    parser.add_argument('--database', help='the tpch database to connect')
    parser.add_argument('--user', help='db user')
    parser.add_argument('--password', help='db password')
    parser.add_argument('--dbdescriptor', help='DBConnection descriptor in Picasso', default='pg')
    parser.add_argument('--picassopath', default='C:\Program Files (x86)\picasso2.1\picasso2.1\PicassoRun\Windows', help='path to folder where runServer.bat is located')
    args = parser.parse_args()
    root = tk.Tk()
    app = App(root, args)
    root.geometry('1500x1000+0+0')
    root.mainloop()
