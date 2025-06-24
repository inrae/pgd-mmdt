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
{ "state": { $type: "string" } },
{ "city": { $type: "string" } },
{ "other": { $type: "string" } },
{ "unit": { $type: "string" } },
{ "west_lon": { $type: "string" } },
{ "east_lon": { $type: "string" } },
{ "north_lat": { $type: "string" } },
{ "south_lat": { $type: "string" } },
{ "climate": { $type: "string" } },
{ "subject": { $type: "array" } },
{ "language": { $type: "array" } },
{ "kindOfData": { $type: "array" } },
{ "dataOrigin": { $type: "array" } },
{ "rice_data": { $type: "array" } },
{ "env_data": { $type: "array" } },
{ "germplasm": { $type: "array" } },
{ "status": { $type: "string" } },
{ "access_rights": { $type: "string" } },
{ "publication_idtype": { $type: "string" } },
{ "recurrent": { $type: "string" } },
{ "trial_frequency": { $type: "string" } },
{ "trial_type": { $type: "string" } },
{ "trial_design": { $type: "string" } },
{ "grain": { $type: "string" } },
{ "grain_quality": { $type: "string" } },
{ "disease": { $type: "string" } },
{ "pest": { $type: "string" } },
{ "agro_practice": { $type: "string" } },
{ "water_management": { $type: "string" } },
{ "datestart": { $type: "string" } },
{ "dateend": { $type: "string" } },
{ "time_period_start": { $type: "string" } },
{ "time_period_end": { $type: "string" } },
{ "description": { $type: "string" } },
{ "note": { $type: "string" } },
{ "publication_citation": { $type: "string" } },
{ "trial_desc": { $type: "string" } },
{ "disease_comment": { $type: "string" } },
{ "pest_comment": { $type: "string" } },
{ "farmers_comment": { $type: "string" } },
{ "comment": { $type: "string" } },
{ "chemin": { $type: "string" } }
			 ]
		  }
	   }
	);
};
