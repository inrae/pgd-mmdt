// Connexion Mongo
conn=new Mongo();
db=conn.getDB("admin");
db=db.getSiblingDB('admin');

// Test existence base mongo pgd-db
if (db.getMongo().getDBNames().indexOf("pgd-db") == -1){
	// Test existence user admin
	if (db.getUser("admin-mongo") == null){
		db.createUser( { user: "admin-mongo", pwd: "aaaaa", roles: [ { role:"userAdminAnyDatabase", db: "admin"}, "readWriteAnyDatabase" ]});
	}
	db=db.getSiblingDB('pgd-db');

	// Test existence user read-write
	if (db.getUser("userw-pgd") == null){
		db.createUser({user: "userw-pgd", pwd: "wwwww", roles: [ { role: "readWrite", db:"pgd-db" } ]});
	}
	// Test existence user read
	if (db.getUser("userr-pgd") == null){
		db.createUser({user: "userr-pgd", pwd: "rrrrr", roles: [ { role: "read", db:"pgd-db" } ]});
	}
	// Création collection metadata
	db.createCollection( "metadata",
	   {
		  validator: { $or:
			 [
{ "title": { $type: "string" } },
{ "fulltitle": { $type: "string" } },
{ "license": { $type: "string" } },
{ "dmpid": { $type: "string" } },
{ "depositor": { $type: "string" } },
{ "publication_idnumber": { $type: "string" } },
{ "publication_url": { $type: "string" } },
{ "subject": { $type: "array" } },
{ "language": { $type: "array" } },
{ "kindOfData": { $type: "array" } },
{ "dataOrigin": { $type: "array" } },
{ "status": { $type: "string" } },
{ "access_rights": { $type: "string" } },
{ "publication_idtype": { $type: "string" } },
{ "datestart": { $type: "string" } },
{ "dateend": { $type: "string" } },
{ "description": { $type: "string" } },
{ "note": { $type: "string" } },
{ "publication_citation": { $type: "string" } },
{ "comment": { $type: "string" } },
{ "chemin": { $type: "string" } }
			 ]
		  }
	   }
	);
};
