import psycopg2

try:
    conn = psycopg2.connect(
        host="localhost",
        database="kitsumadb",
        user="postgres",
        password="Usaid@10",
        port="5432"
    )

    login_id=input("enter your login_id :")
    password=input("enter password :")

    cursor = conn.cursor()
    cursor.execute("SELECT * FROM users WHERE login = %s AND password = crypt(%s, password);",(login_id,password))
    output = cursor.fetchall()
    if output :
        exists=True
    else:
        exists=False
    if exists:
        cursor.execute("select file_content from personalized_files where user_login=%s",(login_id,))
        output=cursor.fetchall()
        with open('structure.txt','w+') as f:
            f.write(output[0][0])
        
    cursor.close()
    conn.close()

except Exception as e:
    print("Error:", e)