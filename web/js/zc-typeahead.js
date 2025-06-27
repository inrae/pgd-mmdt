// Zenodo Communities API with help of Typeahead
function zcTypeahead () {
	$('#push .typeahead').typeahead({
		hint: true,
		highlight: true,
		minLength: 4
	},
	{
		async: true,
		source: function (query, processSync, processAsync) {
			url = ZC_API_URL + encodeURIComponent(query);
			if (DEBUG) console.log('GET '+url)
			return $.ajax({
				url: url,
				type: 'GET',
				dataType: 'json',
				timeout: 5000,
				success: function (json) {
					return processAsync([json]);
				}
			});
		},
		templates: {
			pending: [
				'<div class="empty-message">',
				'<img src="https://seekvectorlogo.com/wp-content/uploads/2023/03/zenodo-vector-logo.png" style="width:50px;"/>&nbsp;&nbsp;'+
				'<b><i>&nbsp;this community seems not to exist...</i></b>',
				'</div>'
			].join('\n'),
			suggestion: function (data) {
				if(data.metadata.title.length > 0)
					return '<p>'+data.metadata.title+'</p>';
			}
		},
		display: function (data) {
			return data.slug;
		},
		value: function(data) {
			return data.slug;
		}
	});
}