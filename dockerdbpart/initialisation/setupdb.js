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
{ "retention_time": { $type: "string" } },
{ "scientific_leader": { $type: "string" } },
{ "leader_orcid": { $type: "string" } },
{ "technical_manager": { $type: "string" } },
{ "manager_orcid": { $type: "string" } },
{ "city": { $type: "string" } },
{ "latitude": { $type: "string" } },
{ "longitude": { $type: "string" } },
{ "valorisation": { $type: "array" } },
{ "scientific_body": { $type: "array" } },
{ "technical_institution": { $type: "array" } },
{ "funder": { $type: "array" } },
{ "contrat": { $type: "array" } },
{ "license": { $type: "array" } },
{ "facility": { $type: "array" } },
{ "site": { $type: "array" } },
{ "years": { $type: "array" } },
{ "living_scale": { $type: "array" } },
{ "living_organism": { $type: "array" } },
{ "species": { $type: "array" } },
{ "variety": { $type: "array" } },
{ "data_type": { $type: "array" } },
{ "phenotypic": { $type: "array" } },
{ "environment": { $type: "array" } },
{ "factors": { $type: "array" } },
{ "status": { $type: "string" } },
{ "access_rights": { $type: "string" } },
{ "description": { $type: "string" } },
{ "chemin": { $type: "string" } }
			 ]
		  }
	   }
	);
};
