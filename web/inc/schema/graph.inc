<?php

$URL=$APPURL."/metadata/$id_metadata/";

?>

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Maggot - JSON-LD Graph Viewer</title>
  <style>
	body { font-family: sans-serif; }
	#controls { margin: 1em 0; display: flex; gap: 2em; align-items: center; }
	#mynetwork { width: 100%; height: 800px; border: 1px solid lightgray; }
  </style>
</head>
<body>

<h2>Maggot - JSON-LD Graph Viewer (<?= $id_metadata ?>)</h2>

<div id="loading" style="display: none; position: absolute; top: 150px; left: 300px; z-index: 9999;">
  <img src="https://i.imgur.com/llF5iyg.gif" alt="Chargement..." style="height:32px; vertical-align:middle;">
  <span>Waiting ...</span>
</div>

<div id="slider" style="display: block; position: absolute; top: 65px; left: 370px; z-index: 9999;">
  <strong>Graph gravity</strong><br>
  <input type="range" id="gravitySlider" min="-100000" max="-1000" step="-1000" value="-30000">
</div>

<div id="slider2" style="display: block; position: absolute; top: 65px; left: 550px; z-index: 9999;">
  <strong>Font size:</strong>&nbsp;<span id="fontValue">24</span><br>
  <input type="range" id="fontSlider" min="8" max="36" step="1" value="24">
</div>

<div id="controls">
  <div>
	<strong>View mode</strong><br>
	<label><input type="radio" name="mode" value="simple" checked> Simple</label>
	<label><input type="radio" name="mode" value="full"> Full</label>
  </div>
  <div>
	<strong>Export</strong><br>
	<select id="exportType">
	  <option value="ntriples">RDF Triples</option>
	  <option value="turtle">RDF Turtle</option>
	  <option value="rdf">RDF/XML</option>
	</select>
	<button id="exportBtn">Export</button>
  </div>
</div>

<div id="legend" style="margin-top: 20px; position: absolute; top: 100px; left: 20px; z-index: 9999;"></div>

<div id="mynetwork"></div>

<script src="https://unpkg.com/jsonld/dist/jsonld.min.js"></script>
<script src="https://unpkg.com/n3/browser/n3.min.js"></script>
<script src="https://unpkg.com/vis-network/standalone/umd/vis-network.min.js"></script>
<script>
	const container = document.getElementById('mynetwork');
	const legendContainer = document.getElementById('legend');
	const loading = document.getElementById('loading');
	const exportBtn = document.getElementById('exportBtn');
	const slider = document.getElementById('gravitySlider');
	const slider2 = document.getElementById('fontSlider');
	const fontValue = document.getElementById('fontValue');

	const colorDict = { // cf https://rgbcolorpicker.com/
		"schema:creator": "#ff9999",
		"schema:author": "#ccffcc",
		"schema:keywords": "#3ec2cc",
		"citation:topicClassification": "#3ec2cc",
		"contributor": "#ccffcc",
		"schema:funding": "#d6d979",
		"schema:hasPart": "#ffcc99",
		"publication": "#ffcc99",
		"citation:producer": "#8e98bf",
		"affiliation": "#bf8ebd",
		"memberOf": "#bf8ebd",
		"funder": "#bf8ebd",
		"default": "#99ccff"
	};

	getJSONLD();

// ------------------

	let jsonldData = null;
	let network = null;

	// Handle view mode switch
	document.querySelectorAll('input[name="mode"]').forEach(radio => {
		radio.addEventListener('change', (e) => {
			loading.style.display = 'block';
			if (jsonldData) renderGraph(e.target.value);
		});
	});

	// Handle export actions
	exportBtn.addEventListener('click', () => exportRDF(), false);

	// Handle slider2 value
	slider2.addEventListener('input', () => {
		fontValue.textContent = parseFloat(slider2.value).toFixed(0);
	});

	// Handle slider change
	slider.addEventListener('change', () => {
		const newGravity = parseFloat(slider.value);
		updateGraphGravity(newGravity);
	});

	// Handle slider2 change
	slider2.addEventListener('change', () => {
		const newFont = parseFloat(slider2.value);
		updateFont(newFont);
	});

	function updateGraphGravity(value) {
		network.setOptions({
			physics: {
				solver: 'barnesHut',
				barnesHut: {
					gravitationalConstant: value
				},
				stabilization: true
			}
		});
	}

	function updateFont(value) {
		network.setOptions({
			nodes: {
				font: { size: value },
				shape: 'dot',
				scaling: { label: true },
				shadow: true
			}
		});
	}

	function getColorForProperty(prop) {
		return colorDict[prop] || colorDict["default"];
	}

	function createLegend(colorDict) {
		legendContainer.innerHTML = '<h4>Legend</h4>';
		const table = document.createElement('table');
		table.style.borderCollapse = 'collapse';
		Object.entries(colorDict).forEach(([property, color]) => {
			const row = document.createElement('tr');
			const colorBox = document.createElement('td');
			colorBox.style.backgroundColor = color;
			colorBox.style.width = '20px';
			colorBox.style.height = '20px';
			colorBox.style.border = '1px solid #000';
			colorBox.style.marginRight = '10px';
			const label = document.createElement('td');
			label.style.paddingLeft = '10px';
			label.textContent = property === 'default' ? 'Other / default' : property;
			row.appendChild(colorBox);
			row.appendChild(label);
			table.appendChild(row);
		});
		legendContainer.appendChild(table);
	}

	function renderGraph(mode = 'simple') {
		const nodes = new vis.DataSet();
		const edges = new vis.DataSet();
		const seen = new Set();
		let anonCounter = 0;

		function getNodeId(obj) {
			if (typeof obj === 'string') return obj;
			if (obj['@id']) return obj['@id'];
			if (obj['name']) return obj['name'];
			return 'anon_' + (++anonCounter);
		}

		function process(obj, parentId = null, prop = null) {
			const currentId = getNodeId(obj);
		
			if (!seen.has(currentId)) {
				const label = obj['name'] || obj['@id'] || obj['description'] || currentId;
				nodes.add({ id: currentId, label: label.toString(), shape: 'ellipse', color: getColorForProperty(prop) });
				seen.add(currentId);
			}
		
			if (parentId && prop) {
				edges.add({ from: parentId, to: currentId, label: prop, color: getColorForProperty(prop) });
			}
		
			for (const key in obj) {
				if (key === '@context') continue;
				const val = obj[key];
			
				if (typeof val === 'object' && val !== null) {
					if (Array.isArray(val)) {
					val.forEach(v => {
						if (typeof v === 'object') {
						process(v, currentId, key);
						} else if (mode === 'full') {
						const literalId = currentId + '_' + key + '_' + v;
						if (!seen.has(literalId)) {
							nodes.add({ id:literalId, label:`${key}: ${v}`, shape:'box', color:getColorForProperty(prop) });
							edges.add({ from: currentId, to: literalId, label: key, color: getColorForProperty(prop) });
							seen.add(literalId);
						}
						}
					});
					} else {
						process(val, currentId, key);
					}
				} else if (mode === 'full' && (typeof val === 'string' || typeof val === 'number')) {
					const literalId = currentId + '_' + key + '_' + val;
					if (!seen.has(literalId)) {
						nodes.add({ id: literalId, label: `${key}: ${val}`, shape: 'box', color: '#f0f0f0' });
						edges.add({ from: currentId, to: literalId, label: key });
						seen.add(literalId);
					}
				}
			}
		}

		process(jsonldData);
		
		const data = { nodes, edges };
		const options = {
			layout: { improvedLayout: true },
			edges: { arrows: 'to', font: { align: 'middle' }, smooth: true },
			nodes: {
				font: { size: parseFloat(slider2.value) },
				shape: 'dot',
				scaling: { label: true },
				shadow: true
			},
			physics: {
				barnesHut: {
					gravitationalConstant: parseFloat(slider.value),
					springLength: 150
				},
				stabilization: true
			}
		};

		network = new vis.Network(container, data, options);
		network.once("stabilizationIterationsDone", function (obj) {
			loading.style.display = "none"
			createLegend(colorDict);
		});
	}

	async function convertJsonLd(format) {
		try {
			const nquads = await jsonld.toRDF(jsonldData, { format: 'application/n-quads' });
			if (format === 'ntriples') return nquads;
	
			if (format === 'turtle') {
				let result = '';
				const parser = new N3.Parser();
				const quads = parser.parse(nquads);
				const writer = new N3.Writer({ format: 'Turtle' });			  
				writer.addQuads(quads);
				writer.end((error, turtle) => { result = turtle; });
			return result;
			}
		} catch (err) {
			console.error("Conversion error :", err);
		}
	}

	async function exportRDF() {
		const format = exportBtn.value;
		if (format==="rdf") {
			loading.style.display = 'block';
			await fetch('<?= $URL ?>?format=rdf')
				.then(response => response.text())
				.then(data => {
					const blob = new Blob([data], { type: 'application/rdf+xml' });
					const url = URL.createObjectURL(blob);
					const link = document.createElement('a');
					link.href = url;
					link.download = 'graph.rdf';
					link.click();
					URL.revokeObjectURL(url);
				
				});
		} else
			await convertJsonLd(format).valueOf(Promise).then(function(data){
				const extension = format === 'turtle' ? 'ttl' : 'nt';
				const blob = new Blob([data], { type: 'text/plain' });
				const url = URL.createObjectURL(blob);
				const link = document.createElement('a');
				link.href = url;
				link.download = 'graph.'+extension;
				link.click();
				URL.revokeObjectURL(url);
		});
		loading.style.display = 'none';
	};

	async function getJSONLD() {
		loading.style.display = 'block';
		await fetch('<?= $URL ?>?format=jsonld')
			.then((res) => res.json())
			.then((data) => {
				jsonldData = data;
				renderGraph('simple');
		});
	}
</script>

</body>
</html>
