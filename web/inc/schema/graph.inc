<?php

$URL=$APPURL."/metadata/$id_metadata/";

?>

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>JSON-LD Graph Viewer + Export</title>
  <script src="https://unpkg.com/jsonld/dist/jsonld.min.js"></script>
  <script src="https://unpkg.com/n3/browser/n3.min.js"></script>
  <script src="https://unpkg.com/vis-network/standalone/umd/vis-network.min.js"></script>
  <style>
    body { font-family: sans-serif; }
    #controls { margin: 1em 0; display: flex; gap: 2em; align-items: center; }
    #mynetwork { width: 100%; height: 800px; border: 1px solid lightgray; }
  </style>
</head>
<body>

<h2>JSON-LD Graph Viewer (<?= $id_metadata ?>)</h2>

<div id="loading" style="display:none; margin-top:1em;">
  <img src="https://i.imgur.com/llF5iyg.gif" alt="Chargement..." style="height:32px; vertical-align:middle;">
  <span>Conversion en cours...</span>
</div>

<div id="controls">
  <div>
    <strong>View mode:</strong><br>
    <label><input type="radio" name="mode" value="simple" checked> Simple</label>
    <label><input type="radio" name="mode" value="full"> Full</label>
  </div>
  <div>
    <strong>Export:</strong><br>
    <select id="exportType">
      <option value="ntriples">RDF Triples</option>
      <option value="turtle">RDF Turtle</option>
      <option value="rdf">RDF/XML</option>
    </select>
    <button id="exportBtn">Export</button>
  </div>
</div>

<div id="mynetwork"></div>

<script>
	const container = document.getElementById('mynetwork');
	let jsonldData = null;
	let currentNetwork = null;
	let currentEdges = null;
	let currentNodes = null;

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
				nodes.add({ id: currentId, label: label.toString(), shape: 'ellipse' });
				seen.add(currentId);
			}
		
			if (parentId && prop) {
				edges.add({ from: parentId, to: currentId, label: prop });
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
							nodes.add({ id: literalId, label: `${key}: ${v}`, shape: 'box', color: '#e0e0e0' });
							edges.add({ from: currentId, to: literalId, label: key });
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
				font: { size: 14 },
				shape: 'dot',
				scaling: { label: true },
				shadow: true
			},
			physics: {
				barnesHut: {
					gravitationalConstant: -30000,
					springLength: 150
				},
				stabilization: true
			}
		};

		currentNetwork = new vis.Network(container, data, options);
		currentNodes = nodes;
		currentEdges = edges;
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

	// Handle view mode switch
	document.querySelectorAll('input[name="mode"]').forEach(radio => {
		radio.addEventListener('change', (e) => {
			if (jsonldData) renderGraph(e.target.value);
		});
	});

	// Handle export actions
	document.getElementById('exportBtn').addEventListener('click', () => exportRDF(), false);

	async function exportRDF() {
		const loader = document.getElementById('loading');
		const format = document.getElementById('exportType').value;
		if (format==="rdf") {
			loader.style.display = 'block';
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
		loader.style.display = 'none';
	};

	fetch('<?= $URL ?>?format=jsonld')
	.then((res) => res.json())
	.then((data) => {
		jsonldData = data;
		renderGraph('simple');
	});

</script>

</body>
</html>
