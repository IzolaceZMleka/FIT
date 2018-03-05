package fit.gja.examples.jax;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.core.Response;
import javax.ws.rs.QueryParam;
import javax.ws.rs.MatrixParam;
import java.util.List;
import javax.ws.rs.DefaultValue;

@Path("/books")
public class JaxParams {


    //URI matching with @Path
    @GET
    public Response getBook() {
        return Response.status(200).entity("Metoda getBook byla zavolána.").build();
    }

    @GET
    @Path("/comic")
    public Response getComicBook() {
        return Response.status(200).entity("Metoda getComicBook byla zavolána.").build();
    }

    //URI matching with parameter
    @GET
    @Path("name/{name}")
    public Response getBookByName(@PathParam("name") String name) {
        return Response.status(200).entity("Metoda getBookByName byla zavolána s parametrem \"" + name + "\"").build();
    }

    @GET
    @Path("releasedate/{day}/{month}/{year}")
    public Response getBookReleaseDate(
            @PathParam("day") int day,
            @PathParam("month") int month,
            @PathParam("year") int year) {

        return Response.status(200)
                .entity("Metoda getBookReleaseDate byla zavolána s parametry: " + day + " " + month + " " + year)
                .build();

    }

    //URI matching regex

    @GET
    @Path("/books/{isbn : ^(?:ISBN(?:-1[03])?:? )?(?=[0-9X]{10}$|(?=(?:[0-9]+[- ]){3})" +
            "[- 0-9X]{13}$|97[89][0-9]{10}$|(?=(?:[0-9]+[- ]){4})[- 0-9]{17}$)" +
            "(?:97[89][- ]?)?[0-9]{1,5}[- ]?[0-9]+[- ]?[0-9]+[- ]?[0-9X]$}")
    public Response getBookByISBN(@PathParam("isbn") String isbn) {

        return Response.status(200)
                .entity("Metoda getBookByISBN byla zavolána s parametrem : " + isbn).build();

    }

    //MatrixParam

    @GET
    @Path("namematrix/{name}")
    public Response getBooksWithParams(@PathParam("name") String name,
                             @MatrixParam("author") String author,
                             @MatrixParam("genre") String genre) {

        return Response
                .status(200)
                .entity("Metoda getBooksWithParams byla zavolána s parametry: jméno: " + genre
                        + ", autor: " + author + ", žánr: " + genre)
                .build();

    }


    //queryparams

    @GET
    @Path("/query")
    public Response getAuthorsBooks(
            @DefaultValue("0") @QueryParam("from") int from,
            @DefaultValue("2018")@QueryParam("to") int to,
            @QueryParam("authors") List<String> authors) {

        return Response
                .status(200)
                .entity("Metoda getAuthorsBooks byla zavolána pro: od: " + from + ", do: " + to
                        + ", autoři: " + authors.toString()).build();
    }

}
