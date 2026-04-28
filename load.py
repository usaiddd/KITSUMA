import psycopg2
import sys

def signup(sign_up, new_pass): 
    try:
        conn = psycopg2.connect(
            host="dpg-d7hp1l57vvec73a3nt3g-a.oregon-postgres.render.com",
            database="kitsumadb_wzpd",
            user="admin1",
            password="k4K6z2M5BMdqR76oIZ8eOH4rSVuQDksr",
            port="5432"
        )
        cursor = conn.cursor()
        cursor.execute("SELECT * FROM users WHERE login = %s;", (sign_up,))
        ifIDexists = cursor.fetchall()
        if (ifIDexists):
            conn.close()
            return 1
        cursor.execute("INSERT INTO users (login, password) VALUES (%s, %s);", (sign_up, new_pass))
        username = sign_up
        conn.commit()
        conn.close()
        return 0 
    except Exception as e:
        return 2
def login(login_id, password): 
    try:
        conn = psycopg2.connect(
            host="dpg-d7hp1l57vvec73a3nt3g-a.oregon-postgres.render.com",
            database="kitsumadb_wzpd",
            user="admin1",
            password="k4K6z2M5BMdqR76oIZ8eOH4rSVuQDksr",
            port="5432"
        )
        cursor = conn.cursor()
        cursor.execute("SELECT * FROM users WHERE login = %s AND password = crypt(%s, password);", (login_id, password))
        output = cursor.fetchall()
        if (output): 
            username = login_id
            conn.close()
            return 0
        else:
            conn.close()
            return 1
    except Exception as e:
        return 2

def deleteuser(user): 
    try:
        conn = psycopg2.connect(
            host="dpg-d7hp1l57vvec73a3nt3g-a.oregon-postgres.render.com",
            database="kitsumadb_wzpd",
            user="admin1",
            password="k4K6z2M5BMdqR76oIZ8eOH4rSVuQDksr",
            port="5432"
        )
        cursor = conn.cursor()
        cursor.execute("delete from users where login = %s;", (user, ))
        conn.commit()
        cursor.close() 
    except Exception as e:
        return 2
    
def putdata(conname, user): 
    try:
        conn = psycopg2.connect(
            host="dpg-d7hp1l57vvec73a3nt3g-a.oregon-postgres.render.com",
            database="kitsumadb_wzpd",
            user="admin1",
            password="k4K6z2M5BMdqR76oIZ8eOH4rSVuQDksr",
            port="5432"
        )   
        cursor = conn.cursor()
        cursor.execute("insert into container (login, container_name) values (%s, %s); ", (user, conname))
        with open("data.txt", "r") as f:
            for line in f:
                line = line.strip()
                if line:
                    userr, password = line.split("|")
                    cursor.execute("SELECT * FROM users WHERE login = %s;", (userr,))
                    ifIDexists = cursor.fetchall()
                    if (ifIDexists):
                        conn.close()
                        return 1
                    cursor.execute("insert into users values (%s, %s);", (userr,password))
        conn.commit()
        conn.close()
        return 0
    except Exception as e:
        return 2
def getdata(login_id): 
    try:
        conn = psycopg2.connect(
            host="dpg-d7hp1l57vvec73a3nt3g-a.oregon-postgres.render.com",
            database="kitsumadb_wzpd",
            user="admin1",
            password="k4K6z2M5BMdqR76oIZ8eOH4rSVuQDksr",
            port="5432"
        )   
        cursor = conn.cursor()
        cursor.execute("SELECT structure from personalized_files where user_login = %s; ", (login_id, ))
        output = cursor.fetchall()
        if (output): 
            with open('structure.txt', 'w+') as f: 
                f.write(output[0][0])
            conn.close()
            return 0
        else:
            conn.close()
            return 1
    except Exception as e:
        return 2
def putfilecontent(filename, file):
    try:
        with open(file, "r") as f:
            filecontent = f.read()
        conn = psycopg2.connect(
            host="dpg-d7hp1l57vvec73a3nt3g-a.oregon-postgres.render.com",
            database="kitsumadb_wzpd",
            user="admin1",
            password="k4K6z2M5BMdqR76oIZ8eOH4rSVuQDksr",
            port="5432"
        )
        cursor = conn.cursor()
        cursor.execute("SELECT filename from file where filename = %s;", (filename, ))
        output = cursor.fetchall()
        if (output): 
            cursor.execute("UPDATE file set file_content = %s where filename = %s;", (filecontent, filename))
        else: 
            cursor.execute("Insert into file (filename, file_content) values (%s, %s);", (filename, filecontent))
        conn.commit()
        conn.close() 
    except Exception as e: 
        return 2    
def getfiledata(file_path,base_path,file_name): 
    try:
        conn = psycopg2.connect(
            host="dpg-d7hp1l57vvec73a3nt3g-a.oregon-postgres.render.com",
            database="kitsumadb_wzpd",
            user="admin1",
            password="k4K6z2M5BMdqR76oIZ8eOH4rSVuQDksr",
            port="5432"
        )
        cursor = conn.cursor()
        cursor.execute("SELECT file_content from file where filename = %s; ", (file_path, ))
        output = cursor.fetchall()
        location=base_path+'/'+file_path
        if (output): 
            with open(location, 'w+') as f: 
                f.write(output[0][0])
            conn.close()
            return 0
        else:
            conn.close()
            return 1
    except Exception as e:
        return 2
def pushes(filename, file, message, user_login): 
    try:
        with open(file, "r") as f:
            filecontent = f.read()
        conn = psycopg2.connect(
            host="dpg-d7hp1l57vvec73a3nt3g-a.oregon-postgres.render.com",
            database="kitsumadb_wzpd",
            user="admin1",
            password="k4K6z2M5BMdqR76oIZ8eOH4rSVuQDksr",
            port="5432"
        )   
        cursor = conn.cursor()
        cursor.execute("SELECT fileno from file where filename = %s;", (filename, ))
        output = cursor.fetchall()
        
        if (output): 
            cursor.execute("INSERT INTO pushes (fileno, file_content, message, user_login) values (%s, %s, %s, %s); ", (output[0][0], filecontent, message, user_login))
            conn.commit() 
            conn.close() 
            return 0;
        else:
            conn.close()
            return 1
    except Exception as e:
        return 2
    
def save_hierarchy(conname):
    try:
        conn = psycopg2.connect(
            host="dpg-d7hp1l57vvec73a3nt3g-a.oregon-postgres.render.com",
            database="kitsumadb_wzpd",
            user="admin1",
            password="k4K6z2M5BMdqR76oIZ8eOH4rSVuQDksr",
            port="5432"
        )
        cursor = conn.cursor()
        cursor.execute("SELECT container_no, login FROM container WHERE container_name = %s ORDER BY container_no DESC LIMIT 1;", (conname,))
        cursor.execute("SELECT container_no, login FROM container WHERE container_name = %s ORDER BY container_no DESC LIMIT 1;", (conname,))
        container_result = cursor.fetchone()
        if not container_result:
            conn.close()
            return 1
        container_no = container_result[0]
        admin_login = container_result[1] 
        admin_login = container_result[1] 
        paths = []
        paths2 = {}
        paths2 = {}
        user_roots = {}
        user_auth = {}
        with open("hierarchy_data.txt", "r") as f:
            for line in f:
                parts = line.strip().split("|")
                if len(parts) < 2: continue
                record_type = parts[0]
                if record_type == "PATH":
                    path_str = parts[1]
                    paths.append(path_str)
                    filename_only = path_str.split('/')[-1]
                    if '.' in filename_only:
                        cursor.execute("SELECT fileno FROM file WHERE filename = %s;", (path_str,))
                        file_result = cursor.fetchone()
                        if not file_result:
                            cursor.execute("INSERT INTO file (filename, file_content) VALUES (%s, '') RETURNING fileno;", (path_str,))
                            fileno = cursor.fetchone()[0]
                        else:
                            fileno = file_result[0]
                        cursor.execute("SELECT 1 FROM containertofile WHERE containerno = %s AND fileno = %s;", (container_no, fileno))
                        if not cursor.fetchone():
                            cursor.execute("INSERT INTO containertofile (containerno, fileno) VALUES (%s, %s);", (container_no, fileno))
                        paths2[path_str] = str(fileno) 
                    else: 
                        paths2[path_str] = path_str
                    path_str = parts[1]
                    paths.append(path_str)
                    filename_only = path_str.split('/')[-1]
                    if '.' in filename_only:
                        cursor.execute("SELECT fileno FROM file WHERE filename = %s;", (path_str,))
                        file_result = cursor.fetchone()
                        if not file_result:
                            cursor.execute("INSERT INTO file (filename, file_content) VALUES (%s, '') RETURNING fileno;", (path_str,))
                            fileno = cursor.fetchone()[0]
                        else:
                            fileno = file_result[0]
                        cursor.execute("SELECT 1 FROM containertofile WHERE containerno = %s AND fileno = %s;", (container_no, fileno))
                        if not cursor.fetchone():
                            cursor.execute("INSERT INTO containertofile (containerno, fileno) VALUES (%s, %s);", (container_no, fileno))
                        paths2[path_str] = str(fileno) 
                    else: 
                        paths2[path_str] = path_str
                elif record_type == "EMP":
                    folder, login = parts[1], parts[2]
                    if login not in user_roots:
                        user_roots[login] = set()
                        user_auth[login] = "E" 
                    user_roots[login].add(folder)
                    cursor.execute("INSERT INTO folder_employees (container_no, folder_path, user_login) VALUES (%s, %s, %s);", 
                                   (container_no, folder, login))
                elif record_type == "HEAD":
                    folder, login = parts[1], parts[2]
                    user_auth[login] = "FH" 
                    cursor.execute("INSERT INTO folder_heads (container_no, folder_path, user_login) VALUES (%s, %s, %s);", 
                                   (container_no, folder, login))
                elif record_type == "HIER":
                    folder, login, rank = parts[1], parts[2], parts[3]
                    cursor.execute("INSERT INTO folder_hierarchy (container_no, folder_path, user_login, rank) VALUES (%s, %s, %s, %s);", 
                                   (container_no, folder, login, int(rank)))
        admin_paths = [paths2[p] for p in paths]
        admin_struct = f"{conname}\nauth:\nA\nstruct:\n" + "\n".join(admin_paths)
        admin_paths = [paths2[p] for p in paths]
        admin_struct = f"{conname}\nauth:\nA\nstruct:\n" + "\n".join(admin_paths)
        cursor.execute("INSERT INTO personalized_files (user_login, structure) VALUES (%s, %s);", (admin_login, admin_struct))
        for login, roots in user_roots.items():
            allowed_paths = []    
            allowed_paths = []    
            for p in paths:
                for r in roots:
                    if p == r or p.startswith(r + "/"):
                        allowed_paths.append(paths2[p])
                        allowed_paths.append(paths2[p])
                        break
            auth_level = user_auth.get(login, "E")
            struct_text = f"{conname}\nauth:\n{auth_level}\nstruct:\n" + "\n".join(allowed_paths)
            cursor.execute("INSERT INTO personalized_files (user_login, structure) VALUES (%s, %s);", (login, struct_text))           
            cursor.execute("INSERT INTO personalized_files (user_login, structure) VALUES (%s, %s);", (login, struct_text))           
        conn.commit()
        conn.close()
        return 0
    except Exception as e:
        print(f"Error: {e}") 
        return 2
        
def getname(fileno):
    try:
        conn = psycopg2.connect(
            host="dpg-d7hp1l57vvec73a3nt3g-a.oregon-postgres.render.com",
            database="kitsumadb_wzpd",
            user="admin1",
            password="k4K6z2M5BMdqR76oIZ8eOH4rSVuQDksr",
            port="5432"
        )
        cursor=conn.cursor()
        cursor.execute("select filename from file where fileno=%s;",(fileno,))
        output=cursor.fetchall()
        filename="temp.txt"
        with open (filename,"w+") as f:
            f.write(output[0][0])
        conn.close()

    except Exception as e:
        print(f"Error: {e}") 
        return 2
    
def check_conflict(fileno):
    try:
        conn = psycopg2.connect(
            host="dpg-d7hp1l57vvec73a3nt3g-a.oregon-postgres.render.com",
            database="kitsumadb_wzpd",
            user="admin1",
            password="k4K6z2M5BMdqR76oIZ8eOH4rSVuQDksr",
            port="5432"
        )
        cursor=conn.cursor()
        cursor.execute("select count(*) from pushes where fileno=%s and merge=false;",(fileno,))
        output=cursor.fetchall()
        num=output[0][0]
        if num>=2:
            cursor.execute("select file_content from pushes where fileno=%s and merge=false;",(fileno,))
            output=cursor.fetchall()
            filename="not_merged/file1"
            with open(filename,"w+") as f:
                f.write(output[0][0])
            filename="not_merged/file2"
            with open(filename,"w+") as f:
                f.write(output[0][1])
            cursor.close()
            conn.close()
            return 1
        else:
            cursor.close()
            conn.close()
            return 0
            
        

    except Exception as e:
        print(f"Error: {e}") 
        return 2

    
if __name__ == "__main__":
    operation = sys.argv[1]

    if operation == "S" or operation == "L":
        user = sys.argv[2]
        pwd = sys.argv[3]

        if operation == "S":
            code = signup(user, pwd)
        else:
            code = login(user, pwd)

    elif operation=="CC":
        fileno= sys.argv[2]
        code = check_conflict(fileno)

    elif operation == "GFN":
        fileno = sys.argv[2]
        code = getname(fileno)

    elif operation == "G":
        user = sys.argv[2]
        code = getdata(user)

    elif operation == "A": 
        filename = sys.argv[2]
        file = sys.argv[3]
        code = putfilecontent(filename, file)
        
    elif operation == "GF":
        file_path=sys.argv[2]
        base_path=sys.argv[3]
        file_name=sys.argv[4]
        code = getfiledata(file_path,base_path,file_name)

    elif operation == "PU": 
        filename = sys.argv[2]
        file = sys.argv[3]
        message = sys.argv[4]
        user_login = sys.argv[5]
        code = pushes(filename, file, message, user_login)

    elif operation == "PD": 
        conname = sys.argv[2]
        user = sys.argv[3]
        code = putdata(conname, user)
    
    elif operation == "X": 
        user = sys.argv[2]
        code = deleteuser(user)

    elif operation == "PH":
        conname = sys.argv[2]
        code = save_hierarchy(conname)
    else:
        code = 2

    sys.exit(code)