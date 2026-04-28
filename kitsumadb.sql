--Hierarchy is better than normal because in this case, the 4th person will have to fix the conflicts of 3 pushes only once, whereas in a traditional case, conflicts (if any) have to be fixed after each push between 2 people


CREATE EXTENSION IF NOT EXISTS pgcrypto;

CREATE OR REPLACE FUNCTION hash_password()
RETURNS TRIGGER AS $$
BEGIN
    NEW.password := crypt(NEW.password, gen_salt('bf'));
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER encrypt_password_trigger
BEFORE INSERT ON users
FOR EACH ROW
EXECUTE FUNCTION hash_password();

CREATE TABLE container(
    login varchar(100) REFERENCES users(login),
    container_no SERIAL PRIMARY KEY,
    container_name varchar(100)
); 

CREATE TABLE containertofile(
    containerno INTEGER REFERENCES container(container_no),
    fileno INTEGER REFERENCES file(fileno), 
);

create table file(
    fileno SERIAL PRIMARY KEY,
    filename TEXT NOT NULL,
    file_content TEXT
);

CREATE TABLE personalized_files (
    file_id SERIAL PRIMARY KEY,
    user_login VARCHAR(100) NOT NULL,
    structure TEXT NOT NULL,

    CONSTRAINT fk_user
        FOREIGN KEY (user_login)
        REFERENCES users(login)
        ON DELETE CASCADE
);

CREATE TABLE pushes(
    push_id SERIAL PRIMARY KEY,
    fileno INTEGER NOT NULL,
    file_content TEXT NOT NULL,
    message TEXT,
    user_login TEXT,
    merged bool, 
    pushed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

    FOREIGN KEY (fileno)
    REFERENCES file(fileno)
    ON DELETE CASCADE,

    FOREIGN KEY (user_login)
    REFERENCES users(login)
    ON DELETE SET NULL
);

CREATE TABLE users (
    login VARCHAR(100) PRIMARY KEY,
    password TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS folder_employees (
    container_no INTEGER REFERENCES container(container_no),
    folder_path TEXT NOT NULL,
    user_login VARCHAR(100) REFERENCES users(login)
);

CREATE TABLE IF NOT EXISTS folder_heads (
    container_no INTEGER REFERENCES container(container_no),
    folder_path TEXT NOT NULL,
    user_login VARCHAR(100) REFERENCES users(login),
    PRIMARY KEY (container_no, folder_path)
);

CREATE TABLE IF NOT EXISTS folder_hierarchy (
    container_no INTEGER REFERENCES container(container_no),
    folder_path TEXT NOT NULL,
    user_login VARCHAR(100) REFERENCES users(login),
    rank INTEGER NOT NULL 
);