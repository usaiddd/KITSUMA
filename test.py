import psycopg2

try:
    conn = psycopg2.connect(
        host="dpg-d7hp1l57vvec73a3nt3g-a.oregon-postgres.render.com",
        database="kitsumadb_wzpd",
        user="admin1",
        password="k4K6z2M5BMdqR76oIZ8eOH4rSVuQDksr",
        port="5432"
    )
    cursor = conn.cursor()
    # cursor.execute("DELETE FROM folder_employees; ")
    # cursor.execute("DELETE FROM folder_heads; ")
    # cursor.execute("DELETE FROM folder_hierarchy; ")
    # cursor.execute("DELETE FROM personalized_files; ")
    # cursor.execute("DELETE FROM CONTAINER; ")
    # cursor.execute("DELETE FROM USERS where login = %s; ", ("Tanmay@MusicPlayer", ))
    cursor.execute("SELECT * from personalized_files; ")
    output = cursor.fetchall()
    print(output)
    conn.commit()
    cursor.close()
    conn.close()

except Exception as e:
    print("Connection failed:")
    print(e)