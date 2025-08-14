create table if not exists users (
    id integer primary key autoincrement,
    user_id text not null,
    hash text not null,
    first_name text not null,
    last_name text not null,
    email text,
    is_admin integer not null default(0)
);
