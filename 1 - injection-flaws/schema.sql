CREATE TABLE gtworld_users (
	account_number INTEGER NOT NULL,
	first_name VARCHAR(50) NOT NULL,
	middle_name VARCHAR(50) DEFAULT '',
	last_name VARCHAR(50) NOT NULL,
	card_pan VARCHAR(30) UNIQUE NOT NULL,
	PRIMARY KEY (account_number)
);

INSERT INTO gtworld_users (account_number, first_name, last_name, card_pan) VALUES(1234567890, 'Drake','Afolabi','1234-****-****-3456');
INSERT INTO gtworld_users (account_number, first_name, last_name, card_pan) VALUES(1234567891, 'JoeBoy','Irawolowo','1234-****-****-3457');
INSERT INTO gtworld_users (account_number, first_name, last_name, card_pan) VALUES(1234567892, 'Olamide','Daoudu','1234-****-****-3458');
INSERT INTO gtworld_users (account_number, first_name, last_name, card_pan) VALUES(1234567893, 'Adekunle','Bronzeli','1234-****-****-3459');
INSERT INTO gtworld_users (account_number, first_name, last_name, card_pan) VALUES(1234567894, 'Fireboy','Waterili','1234-****-****-3450');

SELECT * FROM gtworld_users;