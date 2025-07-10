create table if not exists posts (
    id integer primary key autoincrement,
    title text not null check (length(title) <= 200),
    description text check (length(description) <= 1000),
    filename text not null check (length(filename) <= 200),
    created_on text not null default (datetime('now')),
    updated_on text not null default (datetime('now'))
);
