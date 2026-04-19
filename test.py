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
    conn.commit()
    command = """ SELECT 
        column_name, 
        data_type, 
        character_maximum_length, 
        is_nullable, 
        column_default 
    FROM 
        information_schema.columns 
    WHERE 
        table_name = 'container';
    """
    cursor.execute(command)
    output = cursor.fetchall()
    print(output)
    cursor.close()
    conn.close()

except Exception as e:
    print("Connection failed:")
    print(e)