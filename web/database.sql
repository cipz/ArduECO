CREATE TABLE marker(

    id INTEGER PRIMARY KEY AUTO_INCREMENT ,    
    latitude FLOAT,
    longitude FLOAT,
    time DATETIME,
    route_id INTEGER,
    air FLOAT,
    topic VARCHAR(100)  
    
);
