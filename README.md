<h1><a id="JsonParser2_0"></a>JsonParser2</h1>
<p><strong>A tool for deserializing objects from json files</strong></p>
<h2><a id="Typical_usage_3"></a>Typical usage</h2>
<p>Call the function <code>parsedObject</code> templated on the desired class and give the path to the json file as an argument.</p>
<pre><code>int main()
{
    const auto person = JsonParser::parsedObject&lt;Person&gt;(&quot;SavedPerson.json&quot;);
    std::cout &lt;&lt; person.name &lt;&lt; &quot; is &quot; &lt;&lt; person.age &lt;&lt; std::endl;
}
</code></pre>
<h2><a id="All_you_need_13"></a>All you need</h2>
<p>is…</p>
<ol>
<li>Your <code>class</code></li>
</ol>
<pre><code>struct Person
{
    std::string name;
    int age = 0;
};
</code></pre>
<ol start="2">
<li>Your class should inherit from the class <code>Exposable</code> templated on itself</li>
</ol>
<pre><code>struct Person : Exposable&lt;Person&gt;
{
    std::string name;
    int age = 0;
};
</code></pre>
<ol start="3">
<li>And it must provide a public method <code>expose</code></li>
</ol>
<pre><code>struct Person : Exposable&lt;Person&gt;
{
    std::string name;
    int age = 0;
    static void expose();
};
</code></pre>
<ol start="4">
<li>The body of the method should list all the pairs of <code>tags</code> and <code>pointers to members</code> you want to deserialize</li>
</ol>
<pre><code>struct Person : Exposable&lt;Person&gt;
{
    std::string name;
    int age = 0;
    static void expose()
    {
        Exposable&lt;Person&gt;::expose(&quot;name&quot;, &amp;Person::name);
        Exposable&lt;Person&gt;::expose(&quot;age&quot;, &amp;Person::age);
    }
};
</code></pre>
<p>See <a href="https://github.com/nestoroprysk/JsonParser2/blob/master/Tests/BasicTests.cpp">tests</a> for further examples.</p>
