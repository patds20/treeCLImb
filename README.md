<h1>CLI Tool for Creating ASCII Tree (WORK IN PROGRESS)s</h1>
	<p>This program generates a beautiful and organized trees of your ideas, projects, and tasks using a simple and intuitive interface.</p>
	<h2>Features</h2>
	<ul>
		<li>Easy-to-use command line interface</li>
		<li>Supports nested nodes and max. 5 children</li>
		<li>Prints the trees in CLI</li>
	</ul>
	<h2>Usage</h2>
	<p>First, compile the program using your favorite C++ compiler:</p>
	<pre><code>g++ treeclimb.cpp -o treeclimb</code></pre>
	<p>To generate a mindmap, run the program and enter the root node name:</p>
	<pre><code>./treeclimb init [name of root node] [filename]</code></pre>
	<p>To add a new node to an existing file:</p>
	<pre><code>./treeclimb node [filename] [parent] [label]</code></pre>
	<p>To print the tree to screen:</p>
	<pre><code>./treeclimb print [filename]</code></pre>

