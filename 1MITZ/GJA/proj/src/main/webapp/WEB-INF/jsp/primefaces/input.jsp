<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="PrimeFaces: Input"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main" id="ejb">
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="${pageContext.request.contextPath}"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li>PrimeFaces</li>
                    <li class="active">Input</li>
                </ol>
            </div>
            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">PrimeFaces: Input
                        <span class="pull-right">
                            <a href="${pageContext.request.contextPath}/examples/primefaces/input.xhtml" class="btn btn-success">Živá ukázka</a>
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
                    <div class="panel-heading">Autocomplete</div>
                    <div class="panel-body">
                        <p class="custom-text-block">Komponenta s automatickým doplňováním předdefinovaných hodnot. Hodnoty, které se budou doplňovat jsou uloženy v ManagedBeanu, kterému se zasílá aktuálně napsaný text podle kterého lze generovat vhodné hodnoty pro doplnění (lze ignorovat velká-malá písmena, překlepy apod.).</p>
                        <div class="custom-code-block"><pre class="custom-code"><code class="jsf">&lt;h:form&gt;
    &lt;h:panelGrid columns=&quot;2&quot; cellpadding=&quot;5&quot;&gt;
        &lt;p:outputLabel value=&quot;Simple:&quot; for=&quot;acSimple&quot; /&gt;
        &lt;p:autoComplete id=&quot;acSimple&quot;
                        value=&quot;&num;{autoComplete.text}&quot;
                        completeMethod=&quot;&num;{autoComplete.completeText}&quot; /&gt;

        &lt;p:outputLabel value=&quot;Min Length (3):&quot; for=&quot;acMinLength&quot; /&gt;
        &lt;p:autoComplete id=&quot;acMinLength&quot; minQueryLength=&quot;3&quot;
                        value=&quot;&num;{autoComplete.text}&quot;
                        completeMethod=&quot;&num;{autoComplete.completeText}&quot;
                        effect=&quot;fade&quot; /&gt;

        &lt;p:outputLabel value=&quot;Force Selection:&quot; for=&quot;acForce&quot; /&gt;
        &lt;p:autoComplete id=&quot;acForce&quot; forceSelection=&quot;true&quot;
                        value=&quot;&num;{autoComplete.text}&quot;
                        completeMethod=&quot;&num;{autoComplete.completeText}&quot; /&gt;
    &lt;/h:panelGrid&gt;
&lt;/h:form&gt;</code></pre></div>
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">package example.pf.bean;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;
import javax.faces.bean.ManagedBean;

@ManagedBean
public class AutoComplete implements Serializable {

    private String text;
    private List&lt;String&gt; texts;

    public List&lt;String&gt; completeText(String query) {
        List&lt;String&gt; results = new ArrayList&lt;&gt;();
        for (int i = 0; i &lt; 10; i++) {
            results.add(query + i);
        }
        return results;
    }

    public String getText() {
        return text;
    }

    public void setText(String text) {
        this.text = text;
    }

    public List&lt;String&gt; getTexts() {
        return texts;
    }

    public void setTexts(List&lt;String&gt; texts) {
        this.texts = texts;
    }
}</code></pre></div>
                    </div>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">Calendar</div>
                    <div class="panel-body">
                        <p class="custom-text-block">Pole pro zadání data a nebo času s vyskakovacím kalendářem, kde lze snadno vybrat datum i čas.</p>
                        <div class="custom-code-block"><pre class="custom-code"><code class="jsf">&lt;h:panelGrid columns=&quot;2&quot; cellpadding=&quot;5&quot;&gt;
    &lt;p:outputLabel for=&quot;inline&quot; value=&quot;Inline:&quot; /&gt;
    &lt;p:calendar id=&quot;inline&quot; value=&quot;&quot; mode=&quot;inline&quot; /&gt;

    &lt;p:outputLabel for=&quot;popup&quot; value=&quot;Popup:&quot; /&gt;
    &lt;p:calendar id=&quot;popup&quot; value=&quot;&quot; /&gt;

    &lt;p:outputLabel for=&quot;button&quot; value=&quot;Button:&quot; /&gt;
    &lt;p:calendar id=&quot;button&quot; value=&quot;&quot; showOn=&quot;button&quot; /&gt;

    &lt;p:outputLabel for=&quot;event&quot; value=&quot;Select Event:&quot; /&gt;
    &lt;p:calendar id=&quot;event&quot; value=&quot;&quot;&gt;
        &lt;p:ajax event=&quot;dateSelect&quot; /&gt;
        &lt;!--&lt;p:ajax event=&quot;dateSelect&quot; listener=&quot;&quot; update=&quot;&quot; /&gt;--&gt;
    &lt;/p:calendar&gt;
&lt;/h:panelGrid&gt;</code></pre></div>
                    </div>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-heading">Keyboard</div>
                    <div class="panel-body">
                        <p class="custom-text-block">Pole pro zadanání textu s vyskakovací virtuální klávesnicí.</p>
                        <div class="custom-code-block"><pre class="custom-code"><code class="jsf">&lt;h:form&gt;
    &lt;h:panelGrid columns=&quot;2&quot; cellpadding=&quot;5&quot;&gt;
        &lt;h:outputText value=&quot;Default Layout: &quot; /&gt;
        &lt;p:keyboard id=&quot;default&quot; /&gt;

        &lt;h:outputText value=&quot;Basic QWERTY: &quot; /&gt;
        &lt;p:keyboard id=&quot;qwerty&quot; layout=&quot;qwertyBasic&quot; /&gt;
    &lt;/h:panelGrid&gt;
&lt;/h:form&gt;</code></pre></div>
                    </div>
                </div>
            </div>

                    <div class="row no-margin">
                        <div class="panel panel-primary">
                            <div class="panel-heading">Další komponenty...</div>
                            <div class="panel-body">
                                <div class="custom-code-block"><pre class="custom-code"><code class="jsf">&lt;h2&gt;ColorPicker&lt;/h2&gt;
&lt;h:form&gt;
    &lt;p:colorPicker mode=&quot;inline&quot; /&gt;
&lt;/h:form&gt;

&lt;h2&gt;Password&lt;/h2&gt;
&lt;h:form&gt;
    &lt;p:password id=&quot;feedback&quot; feedback=&quot;true&quot; /&gt;
&lt;/h:form&gt;

&lt;h2&gt;Editor&lt;/h2&gt;
&lt;h:form&gt;
    &lt;p:editor id=&quot;editor&quot; widgetVar=&quot;editorWidget&quot; width=&quot;600&quot; /&gt;
&lt;/h:form&gt;

&lt;h2&gt;Inplace&lt;/h2&gt;
&lt;h:form&gt;
    &lt;h:panelGrid columns=&quot;2&quot; columnClasses=&quot;column&quot; cellpadding=&quot;5&quot;&gt;
        &lt;h:outputLabel for=&quot;basic&quot; value=&quot;Basic:&quot; /&gt;
        &lt;p:inplace id=&quot;basic&quot;&gt;
            &lt;p:inputText value=&quot;Edit Me&quot; /&gt;
        &lt;/p:inplace&gt;

        &lt;h:outputLabel for=&quot;event&quot; value=&quot;Dblclick&quot;/&gt;
        &lt;p:inplace id=&quot;event&quot; label=&quot;Cities&quot; effectSpeed=&quot;fast&quot; event=&quot;dblclick&quot;&gt;
            &lt;h:selectOneMenu&gt;
                &lt;f:selectItem itemLabel=&quot;New York&quot; itemValue=&quot;0&quot; /&gt;
                &lt;f:selectItem itemLabel=&quot;London&quot; itemValue=&quot;1&quot; /&gt;
                &lt;f:selectItem itemLabel=&quot;Paris&quot; itemValue=&quot;2&quot; /&gt;
                &lt;f:selectItem itemLabel=&quot;Berlin&quot; itemValue=&quot;3&quot; /&gt;
            &lt;/h:selectOneMenu&gt;
        &lt;/p:inplace&gt;
    &lt;/h:panelGrid&gt;
&lt;/h:form&gt;

&lt;h2&gt;Knob&lt;/h2&gt;
&lt;p:knob /&gt;

&lt;h2&gt;TriStateCheckbox&lt;/h2&gt;
&lt;h:panelGrid columns=&quot;2&quot; cellpadding=&quot;5&quot;&gt;
    &lt;h:outputText value=&quot;Basic Usage: &quot;/&gt;
    &lt;p:triStateCheckbox value=&quot;&quot;/&gt;

    &lt;h:outputText value=&quot;Ajax Behavior: &quot;/&gt;
    &lt;p:triStateCheckbox id=&quot;ajaxTriState&quot; value=&quot;&quot;&gt;
        &lt;p:ajax event=&quot;change&quot; /&gt;
        &lt;!--&lt;p:ajax event=&quot;change&quot; update=&quot;&quot; listener=&quot;&quot;/&gt;--&gt;
    &lt;/p:triStateCheckbox&gt;

    &lt;h:outputText value=&quot;Customs Icons: &quot;/&gt;
    &lt;p:triStateCheckbox value=&quot;&quot;
                        stateTwoIcon=&quot;ui-icon-plus&quot;
                        stateThreeIcon=&quot;ui-icon-minus&quot;/&gt;

    &lt;p:triStateCheckbox value=&quot;&quot; itemLabel=&quot;Item Label&quot; /&gt;
&lt;/h:panelGrid&gt;</code></pre></div>
                            </div>
                </div>
            </div>

            <jsp:include page="../components/footer.jsp" />
    </body>
</html>