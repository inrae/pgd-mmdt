// ROR API with help of Typeahead
// See https://ror.readme.io/docs/create-ror-powered-typeaheads
//     https://github.com/twitter/typeahead.js/blob/master/doc/jquery_typeahead.md

var ROR_API_URL = "https://api.ror.org/organizations?query="

function rorTypeahead ()
{
	$('#container .ROR').typeahead({
		hint: true,
		highlight: true,
		minLength: 3
	},
	{
		limitget: 50,
		limitview : 50,
		async: true,
		source: function (query, processSync, processAsync) {
			url = ROR_API_URL + encodeURIComponent(query);
			if (DEBUG) console.log('GET '+url)
			return $.ajax({
				url: url,
				type: 'GET',
				dataType: 'json',
				timeout: 5000,
				success: function (json) {
					const q = query.toLowerCase();
					const orgs = json.items
						.map(item => {
							const nameObj = item.names.find(n => n.types.includes('ror_display')) || item.names[0];
							const name = nameObj ? nameObj.value : 'unnamed organization';
							const acronyms = item.names.filter(n => n.types.includes('acronym')).map(n => n.value);
							const country = item.locations?.[0]?.geonames_details?.country_name || '';
							const geoname = item.locations?.[0]?.geonames_details?.name || '';
							const type = item.types.join(', ');
							return {
								id: item.id, name, country, geoname, acronyms, type
							};
						})
					return processAsync(orgs);
				}
			});
		},
		templates: {
			pending: [
				'<div class="empty-message tt-Loading">',
				'<img src="https://files.readme.io/7194034-small-ROR.org_Port_RGB_WhiteBack.png" style="width:50px;"/>&nbsp;&nbsp;'+
				'<b><i>Wait while fetching organizations list ...</i></b>',
				'</div>'
			].join('\n'),
			suggestion: function (data) {
				const acronyms = data.acronyms.length ? `<em>(${data.acronyms.join(', ')})</em>` : '';
				return `<div class="p-1"><strong>${data.name} ${acronyms}</strong><br><small>${data.country} — ${data.geoname} — ${data.type}</small></div>`;
			}
		},
		display: function (data) {
			return data.name.replace(/['"]+/g, '');
		},
		value: function(data) {
			return data.identifier;
		}
	});
}