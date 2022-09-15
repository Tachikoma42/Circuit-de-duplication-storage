drop database circuitStorage if exists;

create database circuitStorage;

use circuitStorage;

create table circuit (
    id int not null auto_increment primary key,
    feature varchar(255) not null,
    location varchar(255) not null,
);
