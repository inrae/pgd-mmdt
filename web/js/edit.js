var newflg = false;
var editflg = false;
var prevalues =[];

var objpre = '';

String.prototype.no_utf8 = function(){
	var accent = [
		/[\300-\306]/g, /[\340-\346]/g, // A, a
		/[\310-\313]/g, /[\350-\353]/g, // E, e
		/[\314-\317]/g, /[\354-\357]/g, // I, i
		/[\322-\330]/g, /[\362-\370]/g, // O, o
		/[\331-\334]/g, /[\371-\374]/g, // U, u
		/[\321]/g, /[\361]/g, // N, n
		/[\307]/g, /[\347]/g, // C, c
	];
	var noaccent = ['A','a','E','e','I','i','O','o','U','u','N','n','C','c'];

	var str = this;
	for(var i = 0; i < accent.length; i++){
		str = str.replace(accent[i], noaccent[i]);
	}

	return str;
}

var getItemForTable = function (item) {
	var content=item;
	var m=item.match(/^(http)/);
	if(m) content='<a target="_blank" href="'+item+'">'+item+'</a>';
	m=item.match(/^\d{4}-\d{4}-\d{4}-\d{3}[0-9|X]$/);
	if(m) content='<a target="_blank" href="https://orcid.org/'+item+'">'+item+'</a>';
	return content;
}

var tableToJson = function (table, header=true) {
	var data = [];
	// first row needs to be headers
	var headers = [];
	var nbcol=table.rows[0].cells.length-1
	for (var i=0; i<nbcol; i++) {
		headers[i] = table.rows[0].cells[i].innerHTML.toLowerCase().replace(/ /gi,'');
	}
	// go through cells
	for (var i=1; i<table.rows.length; i++) {
		var tableRow = table.rows[i];
		var rowData = {};
		for (var j=0; j<nbcol; j++) {
			name=j;
			if (header) name=headers[j];
			rowItem = tableRow.cells[j].innerHTML;
			var m = rowItem.match(/^<a.*>(.*)<\/a>/);
			if (m) { rowData[ name ] = m[1] } else { rowData[ name ] = rowItem };
		}
		data.push(rowData);
	}
	return data;
}


var ErrorValidBox = function(msg) {
	$('#errvalidmsg').html('Error Validation: '+msg)
	$('#errvalid').css('display','block');
}


var select_row = function (inpval) {
	tblobj = jQuery.parseJSON(JSON.stringify(tableToJson(document.getElementById('mytable'),false)))
	var cnt=0;
	tblobj.forEach(function (item){
		$('#line'+cnt).css("display","")
		arr=Object.values(item)
		flg=arr.length
		for (let i = 0; i <arr.length ; i++) {
			value=arr[i].toLowerCase();
			if (value.indexOf(inpval.toLowerCase())>-1) flg--;
		}
		if (flg==arr.length)
			$('#line'+cnt).css("display","none")
		cnt++
	})
	$('#savemsg').html('');
}

var saveData = function (json='') {
	if (json.length==0) json = JSON.stringify(tableToJson(document.getElementById('mytable')));
	json =json.replace(/\(\*\)/g,'');
	//console.log(json);
	ret=0
	$.post( "save", { dico: $('#dico-name').html(), array: json } ).done(function( data ) {
		//console.log( data );
		if (data.indexOf("ERROR") >= 0) {
			data_html = '<font color="red"><i>'+data+'</i></font>';
			ret=1
		} else {
			data_html = '<font color="green"><i>'+data+'</i></font>';
		}
		$('#savemsg').html(data_html);
	});
	return ret;
}

var postDico = function (file) {
	if (!file) return 1;
	var reader = new FileReader();
	reader.readAsText(file, "ISO-8859-1");
	reader.onload = function (evt) {
		//console.log(evt.target.result);
		var errmsg='';
		var errflg=0;
		var data = [];
		var htmlbody='';
		var lines = evt.target.result.split("\n");
		for (var i=0; i<lines.length; i++) {
			if (lines[i].length==0) continue
			htmlbody += '<tr id="line'+i+'">'
			var fields = lines[i].split("\t")
			if (fields.length != regexArray.length) {
				errmsg='Row '+i+' must have '+regexArray.length+' columns';
				errflg=1
				break
			}
			var rowData = {};
			for (var j=0; j<fields.length; j++) {
				if (fields[j].trim().length>0 && ! regexArray[j].test(fields[j].trim().no_utf8())) {
					errmsg=fields[j]+' is not a valid '+nameArray[j];
					errflg=1
					break
				}
				rowData[ nameArray[j] ] = fields[j];
				htmlbody += '<td class="data">'+getItemForTable(fields[j])+'</td>'
			}
			if (errflg>0) break
			data.push(rowData);
			htmlbody += '<td class="td-btn"><button class="btn save">Save</button><button class="btn edit">Edit</button>&nbsp;<button class="btn delete">Del</button><button class="btn cancel">Cancel</button></td></tr>'
		}
		if (errflg==0) {
			//console.log(data);
			var current_table = $('#container').html();
			$('#container').html(htmlbody);
			setTimeout(function(){
				if (confirm('Substitution validation?')==true) {
					ret=saveData(JSON.stringify(data));
				} else {
					ret=1;
				}
				if (ret) $('#container').html(current_table);
			} , 300 );
		} else
			ErrorValidBox(errmsg);
	}
	reader.onerror = function (evt) {
		ErrorValidBox("error reading file");
	}
}

$(document).on('click', '.edit', function() {
	if (!editflg) {
		$('#searchterm').prop('disabled', true);
		prevalues =[];
		$(this).parent().siblings('td.data').each(function() {
			var content = ($(this).children('a').length > 0 ) ? $(this).children('a').text() : $(this).html();
            if (apiArray[$(this).attr('id').match(/\d+/)[0]]=='ROR') {
				$(this).html('<input class="tdico typeahead" value="' + content + '" />');
				callTypeahead();
			}
			else
				$(this).html('<input class="tdico" value="' + content + '" />');
			prevalues.push(content);
		});
		editflg=true
		$(this).hide();
		$(this).siblings('.delete').hide();
		$(this).siblings('.save').show();
		$(this).siblings('.cancel').show();
	}
	$('#savemsg').html('');
});


$(document).on('click', '.btn-errvalid', function() {
	$('#errvalid').css('display','none');
})

$(document).on('click', '.cancel', function()
{
	$('.tt-hint').remove();
	$('input.tdico').each(function(index, element) {
		if (typeof $(this).parent().attr('id') !== "undefined") {
			if (prevalues.length>0) {
				$(this).html(getItemForTable(prevalues.shift()));
				$(this).contents().unwrap();
			}
		} else {
			$(this).parent().parent().html(prevalues.shift());
			objpre = $(this).next();
			$(this).contents().unwrap();
		}
	});
	$( "tr" ).find( "td.data" ).each(function(index, element) {
		var content = $(element).html();
		if (content.match('<input class="tdico" value="">')) $(element).html('');
	});
	$(this).siblings('.save').hide();
	$(this).siblings('.cancel').hide();
	$(this).siblings('.edit').show();
	$(this).siblings('.delete').show();
	$(this).hide();
	$('#errvalid').css('display','none');
	$('#searchterm').prop('disabled', false);
	$('#savemsg').html('');
	editflg=false;
});

$(document).on('click', '.save', function() {
	var allValid=true;
	$('.tt-hint').remove();
	$('input.tdico').each(function(index, element) {
		do {
			if (!allValid) break;
			var content = $(this).val();
			if (content.trim().length==0 && requireArray[index]=='1') {
				ErrorValidBox(nameArray[index]+' is required');
				allValid=false;
				break;
			}
			if (content.trim().length>0 && ! regexArray[index].test(content.no_utf8())) {
				ErrorValidBox(content+' is not a valid '+nameArray[index]);
				allValid=false;
				break;
			}
			break;
		} while(true) 
	});
	if (allValid) {
		$('input.tdico').each(function(index, element) {
			if (typeof $(this).parent().attr('id') !== "undefined") {
				$(this).html(getItemForTable($(this).val()));
				$(this).contents().unwrap();
			} else {
				val = $(this).next()[0].outerText;
				if (val.length==0) val=$(this).val();
				$(this).parent().parent().html(val);
				$(this).contents().unwrap();
			}
		});
		$( "tr" ).find( "td.data" ).each(function(index, element) {
			var content = $(element).html();
			if (content.match('<input class="tdico" value="">')) $(element).html('');
		});
		$(this).siblings('.save').hide();
		$(this).siblings('.cancel').hide();
		$(this).siblings('.edit').show();
		$(this).siblings('.delete').show();
		$(this).hide();
		$('#errvalid').css('display','none');
		editflg=false
		newflg=false
		saveData();
	}
	$('#searchterm').prop('disabled', false);
	$('#savemsg').html('');
});


$(document).on('click', '.delete', function() {
	var value = 'x';
	var editCurrentCell = false;
	if (newflg || editflg) {
		firstCell = $(this).parents('tr').find("input")[0];
		editCurrentCell = typeof firstCell !== "undefined"
		if(editCurrentCell) value = $(firstCell).val();
	}
	badCell = (newflg || editflg) && ! editCurrentCell;
	if (! badCell && (value.length==0 || confirm("Are you sure you want to delete this line ?"))) {
		$(this).parents('tr').remove();
		$('#errvalid').css('display','none');
		editflg=false
		$('#searchterm').prop('disabled', false);
		$('#savemsg').html('');
		if (!newflg) saveData();
		newflg=false
	}
});


$(document).on('click', '.add, .btn-add', function() {
	var tdcols='';
	for (let i = 0; i < nameArray.length; i++) tdcols += '<td class="data"><input class="tdico" value="" /></td>';
	tbl=tableToJson(document.getElementById('mytable'),false)
	$('#container').append('<tr id="line'+tbl.length+'">'+tdcols+'<td><button class="btn save">Save</button><button class="btn edit">Edit</button>&nbsp;<button class="btn delete">Del</button><button class="btn cancel">Cancel</button></td></tr>');
	$("#tbl-main").animate({ scrollTop: $('#tbl-main').prop("scrollHeight")}, 100);
	lastRow =$("#mytable").find("tr").last()[0];
	lastCell = $(lastRow).find("td").last()[0];
	save=$(lastCell).find('.save')[0]; $(save).show()
	edit=$(lastCell).find('.edit')[0]; $(edit).hide()
	newflg=true
	editflg=true
	$('#searchterm').prop('disabled', true);
	$('#savemsg').html('');
});

