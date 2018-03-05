<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="PrimeFaces: Data"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main" id="ejb">
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="${pageContext.request.contextPath}"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li>PrimeFaces</li>
                    <li class="active">Data</li>
                </ol>
            </div>
            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">PrimeFaces: Data
                        <span class="pull-right">
                            <a href="${pageContext.request.contextPath}/examples/primefaces/data.xhtml" class="btn btn-success">Živá ukázka</a>
                        </span>
                    </h1>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-body">
                        <p class="custom-text-block"><strong>Příklady zde obsahují pouze několik zajímavých komponent, kompletní výčet včetně ukázek je v <a href="https://www.primefaces.org/showcase/">dokumentaci PrimeFaces</a>.</strong></p>
                    </div>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">DataList</div>
                    <div class="panel-body">
                        <p class="custom-text-block">Komponenta DataList vykresluje seznam položek v tabulce s automatickým stránkováním. Při vykreslování lze v prvku <code>outputText</code> upravit vykreslení jednotlivých položek, případně vykreslit položky více komponentami (např obrázek a popis).</p>
                        <div class="custom-code-block"><pre class="custom-code"><code class="jsf">&lt;h:form&gt;
    &lt;p:dataList value=&quot;&num;{dataList.data}&quot;
                var=&quot;item&quot; type=&quot;unordered&quot; itemType=&quot;none&quot;
                paginator=&quot;true&quot;
                rows=&quot;10&quot; styleClass=&quot;paginated&quot;&gt;
        &lt;f:facet name=&quot;header&quot;&gt;
            Paginator
        &lt;/f:facet&gt;
        &lt;h:outputText value=&quot;&num;{item}&quot;/&gt;
    &lt;/p:dataList&gt;
&lt;/h:form&gt;</code></pre></div>
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">package example.pf.bean;

import java.util.ArrayList;
import java.util.List;
import javax.annotation.PostConstruct;
import javax.faces.bean.ManagedBean;

@ManagedBean
public class DataList {

    private List&lt;String&gt; data;

    @PostConstruct
    public void init() {
        data = new ArrayList&lt;&gt;();
        for (int i = 0; i &lt; 100; i++) {
            data.add(&quot;Item &quot; + i);
        }
    }

    public List&lt;String&gt; getData() {
        return data;
    }
}</code></pre></div>
                    </div>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">Tree</div>
                    <div class="panel-body">
                        <p class="custom-text-block">Komponenta Tree také vykresluje nějaký seznam položek, ale vykresluje je jako strom. Datová struktura, která se vykresluje musí být kolekce definovaná PrimeFaces.</p>
                        <div class="custom-code-block"><pre class="custom-code"><code class="jsf">&lt;h:form&gt;
    &lt;p:tree value=&quot;&num;{dataTree.root}&quot; var=&quot;node&quot;&gt;
        &lt;p:treeNode&gt;
            &lt;h:outputText value=&quot;&num;{node}&quot; /&gt;
        &lt;/p:treeNode&gt;
    &lt;/p:tree&gt;

    &lt;p:tree value=&quot;&num;{dataTree.root}&quot; var=&quot;node&quot; orientation=&quot;horizontal&quot;&gt;
        &lt;p:treeNode&gt;
            &lt;h:outputText value=&quot;&num;{node}&quot; /&gt;
        &lt;/p:treeNode&gt;
    &lt;/p:tree&gt;
&lt;/h:form&gt;
</code></pre></div>
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">package example.pf.bean;

import javax.annotation.PostConstruct;
import javax.faces.bean.ManagedBean;
import org.primefaces.model.DefaultTreeNode;
import org.primefaces.model.TreeNode;

@ManagedBean
public class DataTree {

    private TreeNode root;

    @PostConstruct
    public void init() {
        root = new DefaultTreeNode(&quot;Root&quot;, null);
        TreeNode node0 = new DefaultTreeNode(&quot;Node 0&quot;, root);
        TreeNode node1 = new DefaultTreeNode(&quot;Node 1&quot;, root);

        TreeNode node00 = new DefaultTreeNode(&quot;Node 0.0&quot;, node0);
        TreeNode node01 = new DefaultTreeNode(&quot;Node 0.1&quot;, node0);

        TreeNode node10 = new DefaultTreeNode(&quot;Node 1.0&quot;, node1);

        node1.getChildren().add(new DefaultTreeNode(&quot;Node 1.1&quot;));
        node00.getChildren().add(new DefaultTreeNode(&quot;Node 0.0.0&quot;));
        node00.getChildren().add(new DefaultTreeNode(&quot;Node 0.0.1&quot;));
        node01.getChildren().add(new DefaultTreeNode(&quot;Node 0.1.0&quot;));
        node10.getChildren().add(new DefaultTreeNode(&quot;Node 1.0.0&quot;));
        root.getChildren().add(new DefaultTreeNode(&quot;Node 2&quot;));
    }

    public TreeNode getRoot() {
        return root;
    }
}</code></pre></div>
                    </div>
                </div>
            </div>

            <jsp:include page="../components/footer.jsp" />
    </body>
</html>