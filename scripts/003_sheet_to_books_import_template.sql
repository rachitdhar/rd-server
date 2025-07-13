/*

TITLE: Sheet To Books Import Template
CREATED ON: July 13, 2025
AUTHOR: Rachit Dhar

NOTE:
	This is only a template script.
	The actual values that are to be inserted into the books table
	need to be manually written below the INSERT statement below.

	DO NOT RUN THIS SCRIPT DIRECTLY AS IT IS.
*/

CREATE TABLE temp_sheet_to_books_mapping (
    author_name TEXT,
    name TEXT,
    status_code TEXT,
    classification_code TEXT
);

INSERT INTO temp_sheet_to_books_mapping (author_name, name, status_code, classification_code) VALUES
/*
	*** WRITE VALUES HERE ***

	EXAMPLE:

	('Dante Alighieri', 'Divine Comedy: Inferno, Purgatorio, Paradiso', 'CR', 'FIC'),
	('Homer', 'The Iliad', 'NR', 'FIC'),
	('Douglas Adams', 'The Ultimate Hitchhiker''s Guide to the Galaxy', 'NR', 'FIC')
*/
;

INSERT INTO books (name, author_name, status_id, classification_id)
SELECT
    stbm.name,
    stbm.author_name,
    s.id AS status_id,
    c.id AS classification_id
FROM temp_sheet_to_books_mapping stbm
JOIN book_status s ON stbm.status_code = s.code
JOIN book_classification c ON stbm.classification_code = c.code;

DROP TABLE temp_sheet_to_books_mapping;
