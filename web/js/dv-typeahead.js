// Dataverse Search API with help of Typeahead
function dvTypeahead () {
	$('#push .typeahead').typeahead({
		hint: true,
		highlight: true,
		minLength: 3
	},
	{
		limitget: 50,
        limitview : 50,
		async: true,
		source: function (query, processSync, processAsync) {
			url = DV_API_URL + '?type=dataverse&q=' + encodeURIComponent(query)+'*';
			if (DEBUG) console.log('GET '+url)
			return $.ajax({
				url: url,
				type: 'GET',
				dataType: 'json',
				success: function (json) {
					orgs = json.data.items
					return processAsync(orgs);
				}
			});
		},
		templates: {
			pending: [
				'<div class="empty-message">',
				'<img src="https://guides.dataverse.org/en/latest/_images/dataverse-icon.jpg" style="width:30px;"/>&nbsp;&nbsp;'+
				'<b><i>&nbsp; ... wait ...</i></b>',
				'</div>'
			].join('\n'),
			empty: [
				'<div class="empty-message">',
				'<img src="https://guides.dataverse.org/en/latest/_images/dataverse-icon.jpg" style="width:30px;"/>&nbsp;&nbsp;'+
				'<b><i>&nbsp;this dataverse seems not to exist...</i></b>',
				'</div>'
			].join('\n'),
			suggestion: function (data) {
				return '<p>'+data.name+'</p>';
			}
		},
		display: function (data) {
			return data.identifier;
		},
		value: function(data) {
			return data.identifier;
		}
	});
}