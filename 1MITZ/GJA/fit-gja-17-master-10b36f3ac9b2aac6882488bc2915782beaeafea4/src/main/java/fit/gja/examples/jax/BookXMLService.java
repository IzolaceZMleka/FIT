package fit.gja.examples.jax;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;

@Path("/book/xml")
public class BookXMLService {

    @GET
    @Path("/get")
    @Produces("application/xml")
    public Book getBookInXML() {

        Book book = new Book();
        book.setTitle("Saturnin");
        book.setAuthor("Zdenek Jirotka");
        book.setISBN(123456789);

        return book;

    }

}

