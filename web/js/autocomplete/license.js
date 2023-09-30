// Ontostack
//apiurl='http://vocab.fairdatacollective.org/rest/v1/fdc-gdmt'
//parent = 'http://vocab.fairdatacollective.org/gdmt/LicenseIdentifier'

// LOTERRE
//apiurl='https://skosmos.loterre.fr/rest/v1/TSO'
//parent = 'http://data.loterre.fr/ark:/67375/TSO-WZ0J1023-D'

//license=[]; get_terms_from_skosmos(apiurl, [], 'license', lang='en',parent=parent)


// https://spdx.org/licenses/
// https://github.com/spdx/license-list-data

license=[];

var json=[];
var url='https://raw.githubusercontent.com/spdx/license-list-data/main/json/licenses.json';
var patterns = [ /^CC-BY-/, /^etalab/, /^GPL-/, /^MIT-/, /^CECILL-/ ];

$.ajax({ type: 'GET', 
	url: url, 
	async: false,
	header: { 'accept': 'application/json', 'Access-Control-Allow-Origin': '*' },
	success: function(data){ json=JSON.parse(data);  }
})

setTimeout(function(){
   for(var i=0;i<json['licenses'].length;i++) {
      name=json['licenses'][i]['licenseId'];
      if (name.length>15) continue
      for (var item in patterns)
          if (name.match(patterns[item])) { license.push(name); break; }
   }
}, 300);



