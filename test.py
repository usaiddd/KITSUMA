# import psycopg2

# try:
#     conn = psycopg2.connect(
#         host="dpg-d7hp1l57vvec73a3nt3g-a.oregon-postgres.render.com",
#         database="kitsumadb_wzpd",
#         user="admin1",
#         password="k4K6z2M5BMdqR76oIZ8eOH4rSVuQDksr",
#         port="5432"
#     )
#     cursor = conn.cursor()
#     cursor.execute("SELECT * from file; ")
#     output = cursor.fetchall()
#     print(output)
#     conn.commit()
#     cursor.close()
#     conn.close()

# except Exception as e:
#     print("Connection failed:")
#     print(e)

import psycopg2
def reset_database():
    try:
        print("Connecting to database...")
        conn = psycopg2.connect(
            host="dpg-d7hp1l57vvec73a3nt3g-a.oregon-postgres.render.com",
            database="kitsumadb_wzpd",
            user="admin1",
            password="k4K6z2M5BMdqR76oIZ8eOH4rSVuQDksr",
            port="5432"
        )
        cursor = conn.cursor()
        
        print("Executing TRUNCATE command...")
        cursor.execute("""
            TRUNCATE TABLE 
                users, 
                container, 
                file, 
                containertofile, 
                personalized_files, 
                pushes, 
                folder_employees, 
                folder_heads, 
                folder_hierarchy 
            RESTART IDENTITY CASCADE;
        """)
        
        conn.commit()
        conn.close()
        print("✅ Success! All tables have been completely wiped and IDs reset to 1.")
        print("Your database is now completely clean and ready for a fresh test.")

    except Exception as e:
        print(f"❌ Error resetting database: {e}")

if __name__ == "__main__":
    confirmation = input("Are you sure you want to WIPE ALL DATA? (type 'yes' to confirm): ")
    if confirmation.lower() == 'yes':
        reset_database()
    else:
        print("Database reset aborted.")