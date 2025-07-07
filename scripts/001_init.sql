create table if not exists book_status (
    id integer primary key autoincrement,
    code text not null check (length(code) <= 20),
    name text not null check (length(name) <= 100)
);

insert into book_status (code, name) values
('NR', 'Not Read'),
('CR', 'Currently Reading'),
('RD', 'Read');

create table if not exists book_classification (
    id integer primary key autoincrement,
    code text not null check (length(code) <= 20),
    name text not null check (length(name) <= 50)
);

insert into book_classification (code, name) values
('FIC', 'Fiction'),
('NFIC', 'Non Fiction'),
('ACAD', 'Academic');

create table if not exists books (
    id integer primary key autoincrement,
    name text not null check (length(name) <= 200),
    author_name text not null check (length(author_name) <= 100),
    status_id integer not null default (1),
    classification_id integer not null,
    created_on text default (datetime('now')),
    updated_on text default (datetime('now')),
    foreign key (status_id) references book_status(id),
    foreign key (classification_id) references book_classification(id)
);
