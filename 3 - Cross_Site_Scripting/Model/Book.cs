using Microsoft.AspNetCore.Authentication;
using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;
using System.Text.RegularExpressions;

namespace BookListRazor.Model
{
    public class Book
    {
        [Key]
        public int Id { get; set; }
        [Required]

        //[RegularExpression(@"^[a-zA-Z]+$", ErrorMessage = "Use letters only please")] // allow just alphabet
        public string Name { get; set; }
       
        [Required]
     //  [RegularExpression(@"[a-zA-Z]+$", ErrorMessage = "Use letters only please")] // allow just alphabet
        [Display(Name="Author ")]
        public string Author { get; set; }

        public string ISBN { get; set; }  // this is a new update to database u run new migration e.g Pm AddISBNToBookModel , then update the database

    }
}
