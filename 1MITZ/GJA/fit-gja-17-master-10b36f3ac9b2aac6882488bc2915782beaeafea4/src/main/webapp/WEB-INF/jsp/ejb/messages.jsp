<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="EJB: Message driven"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main" id="ejb">
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="${pageContext.request.contextPath}"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li>EJB</li>
                    <li class="active"> Message driven</li>
                </ol>
            </div>
            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">EJB:  Message driven
                        <a href="${pageContext.request.contextPath}/example/ejb-all.zip" class="pull-right btn btn-success">Zdrojové kódy</a>
                    </h1>
                </div>
            </div>

            <div class="row no-margin">
                <div class="panel panel-primary">
                    <div class="panel-body">
                        <p class="custom-text-block">Message driven beany (řízené zprávami) komunikují s ostatními pomocí zpráv, které se zasílají pomocí JMS. Na rozdíl od session beanů (stateless a stateful) nemají žádné remote nebo local rozhraní, ale implementují rozhraní <code>MessageListener</code> a komunikace s nimi probíhá přes zasílané zprávy. Tyto beany podobně jako stateless beany nezachovávají svůj stav (každá zpráva může být přijata jinou instancí).</p>
                        <p class="custom-text-block">Zprávy zaslané do fronty bean zpracuje metodou <code>void onMessage(Message message)</code>. V této metodě lze zasílat zprávy do fonty, něco zpracovávat v databázi apod.</p>
                        <div class="custom-code-block"><pre class="custom-code"><code class="java">package example.ejb;

import javax.annotation.Resource;
import javax.ejb.MessageDriven;
import javax.jms.*;

@MessageDriven
public class ChatBean implements MessageListener {

    @Resource
    private ConnectionFactory connectionFactory;
    @Resource(name = "ChatQueue")
    private Queue chatQueue;

    public ChatBean() {
    }

    @Override
    public void onMessage(Message message) {
        try {
            final String text = ((TextMessage) message).getText();
            send("Reply to '" + text + "'");
        } catch (JMSException ex) {
            // empty...
        }
    }

    private void send(String text) throws JMSException {
        try (Connection connection = connectionFactory.createConnection();
             Session session = connection.createSession(false, Session.AUTO_ACKNOWLEDGE)) {
            connection.start();

            // Create a MessageProducer from the Session to the Topic or Queue
            MessageProducer producer = session.createProducer(chatQueue);
            producer.setDeliveryMode(DeliveryMode.NON_PERSISTENT);

            // Create a message
            TextMessage message = session.createTextMessage(text);

            // Tell the producer to send the message
            producer.send(message);
        }
    }
}</code></pre></div>
                    </div>
                </div>
            </div>
        </div>

        <jsp:include page="../components/footer.jsp" />
    </body>
</html>