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
    """ cursor.execute("delete from users where login = %s", ("Tanmay@MusicPlayer", ))
    conn.commit()  """
    cursor.execute("select * from users; ")
    output = cursor.fetchall()
    print(output)
    cursor.close()
    conn.close()

except Exception as e:
    print("Connection failed:")
    print(e)