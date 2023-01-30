var cities=[];
$.getJSON("https://geo.api.gouv.fr/communes", function (data) {
    $.each(data, function (index, value) { cities.push(value['nom']); });
});
