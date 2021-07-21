SELECT people.name FROM people JOIN 
		(SELECT stars.person_id AS pid FROM stars JOIN movies ON stars.movie_id == movies.id 
		WHERE movies.title == "Toy Story") 
ON pid == people.id;