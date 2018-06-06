#include "parseConfig.h"

int main(int argc, char **argv)
{
	char *filename;
	FILE *fp;

	/*checkArguments(argc, 2);*/

	filename = malloc(sizeof(char)* (strlen(argv[1]) + 5));
	filename[0] = '\n';

	strcpy(filename, argv[1]);

	fp = fopen(filename, "r");

	if (strstr(filename, ".wpml") != NULL){
	filename[strlen(filename) - 5] = '\0';
	}
	strcat(filename, ".php\0");

	parse(fp, filename);

	fclose(fp);
	free(filename);
	return 0;
}

void parse(FILE *fp, char *nameOfFile)
{
	FILE *file;
	char *line;
	int longestLine, numCom;

	if (strcmp(nameOfFile, "index.php") != 0)
	{
		file = fopen(nameOfFile, "w");
	}

	longestLine = longestPossibleLine(fp);
	numCom = numCommas(fp);

	line = malloc(sizeof(char) * longestLine);
	memset(line, 0, strlen(line));
	line[0] = '\0';

	while (fgets(line, longestLine, fp) != NULL)
	{
		int i;

		for (i = 0; i < strlen(line); i++)
		{
			if (line[i] == '.')
			{
				i++;
				/*Addauthor*/
				if (line[i] == 'a')
				{
					fprintf(file, "<?php\n");
					fprintf(file, "    exec('./parseConfig add.wpml');\n");
					fprintf(file, "    exec('./parseConfig remove.wpml');\n");
					fprintf(file, "?>\n");
				}
				/*Display message based on current message*/
				else if (line[i] == 'v')
				{
					fprintf(file, "<?php\n");
					fprintf(file, "    $msg = $_POST['currentMessage'];\n");
					fprintf(file, "    $stream = $_POST['stream'];\n");
					fprintf(file, "    $user = $_POST['username'];\n");
					fprintf(file, "    echo \"Currently viewing stream \\\"$stream\\\" as $user.<br>\";\n");
					fprintf(file, "    echo exec(\"./printPost.py $msg \" . \". $stream \" . \". $user\");\n");
					fprintf(file, "?>\n");
				}
				/*Display streams for user*/
				else if (line[i] == '4')
				{
					fprintf(file, "<?php\n");
					fprintf(file, "    $user = $_POST['username'];\n");
					fprintf(file, "    exec('./parseConfig checkstreams.wpml');\n");
					fprintf(file, "    echo \"User, $user, is a member of:<br>\";\n");
					fprintf(file, "    echo exec(\"./getStreams.py $user\");\n");
					fprintf(file, "?>\n");
				}
				/*Check stream and get last unread post*/
				else if (line[i] == '5')
				{
					fprintf(file, "<?php\n");
					fprintf(file, "    $user = $_POST['username'];\n");
					fprintf(file, "    $stream = $_POST['stream'];\n");
					fprintf(file, "    exec('./parseConfig view.wpml');\n");
					fprintf(file, "    echo \"Selected stream, $stream, as user, $user.<br>\";\n");
					fprintf(file, "    exec(\"./getLastUnread.py $user . $stream\", $output);\n");
					fprintf(file, "    if ($output[0] == \"-1\") {");
					fprintf(file, "        echo \"User does not exist in selected stream.<br>No posts to view.<br><hr>\";\n");
					fprintf(file, "    }\n");
					fprintf(file, "    else {\n");
					fprintf(file, "        echo '<hr><form action=\"view.php\" method=\"POST\">';\n");
					fprintf(file, "        echo \"<input type=\\\"hidden\\\" name=\\\"username\\\" value=\\\"$user\\\">\";\n");
					fprintf(file, "        echo \"<input type=\\\"hidden\\\" name=\\\"currentMessage\\\" value=\\\"$output[0]\\\">\";\n");
					fprintf(file, "        echo \"<input type=\\\"hidden\\\" name=\\\"stream\\\" value=\\\"$stream\\\">\";\n");
					fprintf(file, "        echo '<input type=\"submit\" value=\"View\">';\n");
					fprintf(file, "        echo '</form>';\n");
					fprintf(file, "    }\n");
					fprintf(file, "?>\n");
				}
				/*Create 3 hidden vars with button for use with view*/
				else if (line[i] == '6')
				{
					fprintf(file, "<?php\n");
					fprintf(file, "    $msg = $_POST['currentMessage'];\n");
                                        fprintf(file, "    $stream = $_POST['stream'];\n");
                                        fprintf(file, "    $user = $_POST['username'];\n");
					fprintf(file, "    exec(\"./getLastUnread.py $user . $stream\", $output);\n");
                                        fprintf(file, "    echo '<form action=\"view.php\" method=\"POST\">';\n");
                                        fprintf(file, "    echo \"<input type=\\\"hidden\\\" name=\\\"username\\\" value=\\\"$user\\\">\";\n");
					fprintf(file, "    echo \"<input type=\\\"hidden\\\" name=\\\"stream\\\" value=\\\"$stream\\\">\";\n");
					fprintf(file, "    echo \"<input type=\\\"hidden\\\" name=\\\"currentMessage\\\" value=\\\"$msg\\\">\";\n");
                                        fprintf(file, "    echo '<input type=\"submit\" value=\"Check For New Posts\">';\n");
                                        fprintf(file, "    echo '</form>';\n");
					fprintf(file, "?>\n");
				}
				/*Previous*/
				else if (line[i] == '7')
				{
					fprintf(file, "<?php\n");
                                        fprintf(file, "    $msg = $_POST['currentMessage'];\n");
                                        fprintf(file, "    $stream = $_POST['stream'];\n");
                                        fprintf(file, "    $user = $_POST['username'];\n");
                                        fprintf(file, "    echo '<form action=\"view.php\" method=\"POST\">';\n");
                                        fprintf(file, "    echo \"<input type=\\\"hidden\\\" name=\\\"username\\\" value=\\\"$user\\\">\";\n");
                                        fprintf(file, "    echo \"<input type=\\\"hidden\\\" name=\\\"stream\\\" value=\\\"$stream\\\">\";\n");
                                        fprintf(file, "    echo \"<input type=\\\"hidden\\\" name=\\\"currentMessage\\\" value=\\\"$msg\\\">\";\n");
                                        fprintf(file, "    echo \"<input type=\\\"hidden\\\" name=\\\"x\\\" value=\\\"prev\\\">\";\n");
					fprintf(file, "    echo '<input type=\"submit\" value=\"Prev\">';\n");
                                        fprintf(file, "    echo '</form>';\n");
                                        fprintf(file, "?>\n");
				}
				/*Previous*/
                                else if (line[i] == '8')
                                {
                                        fprintf(file, "<?php\n");
                                        fprintf(file, "    $msg = $_POST['currentMessage'];\n");
                                        fprintf(file, "    $stream = $_POST['stream'];\n");
                                        fprintf(file, "    $user = $_POST['username'];\n");
                                        fprintf(file, "    echo '<form action=\"view.php\" method=\"POST\">';\n");
                                        fprintf(file, "    echo \"<input type=\\\"hidden\\\" name=\\\"username\\\" value=\\\"$user\\\">\";\n");
                                        fprintf(file, "    echo \"<input type=\\\"hidden\\\" name=\\\"stream\\\" value=\\\"$stream\\\">\";\n");
                                        fprintf(file, "    echo \"<input type=\\\"hidden\\\" name=\\\"currentMessage\\\" value=\\\"$msg\\\">\";\n");
                                        fprintf(file, "    echo \"<input type=\\\"hidden\\\" name=\\\"x\\\" value=\\\"next\\\">\";\n");
                                        fprintf(file, "    echo '<input type=\"submit\" value=\"Next\">';\n");
                                        fprintf(file, "    echo '</form>';\n");
                                        fprintf(file, "?>\n");
                                }
				/*Handle prev next mark all*/
				else if (line[i] == '9')
				{
					fprintf(file, "<?php\n");
					fprintf(file, "    $msg = $_POST['currentMessage'];\n");
                                        fprintf(file, "    $stream = $_POST['stream'];\n");
                                        fprintf(file, "    $user = $_POST['username'];\n");
					fprintf(file, "    $task = $_POST['x'];\n");
					fprintf(file, "    if ($task == 'next') {\n");
					fprintf(file, "    exec(\"./next.py $msg \" . \". $stream \" . \". $user\", $output);\n");
					fprintf(file, "    $_POST['currentMessage'] = $output[0];\n}\n");
					fprintf(file, "    else if ($task == 'prev') {\n");
					fprintf(file, "    exec(\"./prev.py $msg \" . \". $stream \" . \". $user\", $output);\n");
                                        fprintf(file, "    $_POST['currentMessage'] = $output[0];\n}\n");
					fprintf(file, "    else if ($task == 'mark') {\n");
                                        fprintf(file, "    exec(\"./markall.py $msg \" . \". $stream \" . \". $user\", $output);\n");
                                        fprintf(file, "    }\n");
					fprintf(file, "?>\n");
				}
				/*Handle mark all*/
				else if (line[i] == 'q')
				{
					fprintf(file, "<?php\n");
                                        fprintf(file, "    $msg = $_POST['currentMessage'];\n");
                                        fprintf(file, "    $stream = $_POST['stream'];\n");
                                        fprintf(file, "    $user = $_POST['username'];\n");
                                        fprintf(file, "    echo '<form action=\"view.php\" method=\"POST\">';\n");
                                        fprintf(file, "    echo \"<input type=\\\"hidden\\\" name=\\\"username\\\" value=\\\"$user\\\">\";\n");
                                        fprintf(file, "    echo \"<input type=\\\"hidden\\\" name=\\\"stream\\\" value=\\\"$stream\\\">\";\n");
                                        fprintf(file, "    echo \"<input type=\\\"hidden\\\" name=\\\"currentMessage\\\" value=\\\"$msg\\\">\";\n");
                                        fprintf(file, "    echo \"<input type=\\\"hidden\\\" name=\\\"x\\\" value=\\\"mark\\\">\";\n");
                                        fprintf(file, "    echo '<input type=\"submit\" value=\"Mark All Read\">';\n");
                                        fprintf(file, "    echo '</form>';\n");
                                        fprintf(file, "?>\n");
				}
				/*Post*/
				if (line[i] == 'o')
				{
					fprintf(file, "<?php\n");
					fprintf(file, "    exec('./parseConfig submitpost.wpml')\n");
					fprintf(file, "?>\n");
				}
				/*submit post*/
                if (line[i] == 's')
              	{
                	fprintf(file, "<?php\n");
					fprintf(file, "    $user = $_POST['username'];\n");
					fprintf(file, "    $stream = $_POST['stream'];\n");
					fprintf(file, "    $post = $_POST['userpost'];\n");
					fprintf(file, "    $f = fopen(\"posttext\", \"w\");\n");
					fprintf(file, "    fwrite($f, $stream);\n");
					fprintf(file, "    fwrite($f, \"\\n\");\n");
					fprintf(file, "    fwrite($f, $post);\n");
					fprintf(file, "    fclose($f);\n");
					fprintf(file, "    echo exec(\"./post $user\");\n");
					fprintf(file, "?>\n");
               	}
				/*add*/
				else if (line[i] == '1')
				{
					fprintf(file, "<?php\n");
                                        fprintf(file, "    $user = $_POST['author'];");
					fprintf(file, "    $f = fopen(\"addremovetext\", \"w\");\n");
					fprintf(file, "    fwrite($f, $_POST['streams']);\n");
					fprintf(file, "    fclose($f);\n");
					fprintf(file, "    echo exec(\"./addauthor $user\");\n");
					fprintf(file, "?>\n");
				}
				/*remove*/
				else if (line[i] == '2')
				{
					fprintf(file, "<?php\n");
					fprintf(file, "    $user = $_POST['author'];");
					fprintf(file, "    $f = fopen(\"addremovetext\", \"w\");\n");
					fprintf(file, "    fwrite($f, $_POST['streams']);\n");
					fprintf(file, "    fclose($f);\n");
					fprintf(file, "    echo exec(\"./addauthor -r $user\");\n");
					fprintf(file, "?>\n");
				}
				/*Index*/
				else if (line[i] == 'z')
				{
					printf("<h3>Login</h3><hr>");
					printf("<form action=\"checkid.php\" method=\"POST\">");
					printf("User: <input type=\"text\" name=\"username\" value=\"\"><br>");
					printf("<input type=\"submit\" value=\"Login\"></form>");
				}
				/*Check login*/
				else if (line[i] == 'u')
				{
					fprintf(file, "<?php\n");
					fprintf(file, "    $user = $_POST['username'];\n");
					fprintf(file, "    $cmd = \"./checkLogin.py $user\";\n");
					fprintf(file, "    exec('./parseConfig home.wpml');\n");
					fprintf(file, "    exec('./parseConfig invaliduser.wpml');\n");
					fprintf(file, "    exec($cmd, $output, $status);\n");
					fprintf(file, "    if ($status)\n");
					fprintf(file, "        echo \"Exec failed\";\n");
					fprintf(file, "    else {\n");
					fprintf(file, "        for ($i = 0; $i < count($output); $i++)\n");
					fprintf(file, "        {\n");
					fprintf(file, "            if ($output[$i] == \"1\")\n");
					fprintf(file, "            {\n");
					fprintf(file, "                echo \"<hr>User \\\"$user\\\" exists.<hr>\";\n");
					fprintf(file, "                echo '<form action=\"home.php\" method=\"POST\">';\n");
					fprintf(file, "                echo \"<input type=\\\"hidden\\\" name=\\\"username\\\" value=\\\"$user\\\">\";\n");
					fprintf(file, "                echo '<input type=\"submit\" value=\"Continue\">';\n");
					fprintf(file, "                echo '</form>';\n");
					fprintf(file, "            }\n");
					fprintf(file, "            else if ($output[$i] == \"0\")\n");
					fprintf(file, "            {\n");
					fprintf(file, "                echo \"<hr>User \\\"$user\\\" does not exist, click below to try again or add/remove author<hr><br>\";\n");
					fprintf(file, "                echo '<form action=\"invaliduser.php\" method=\"POST\">';\n");
					fprintf(file, "                echo \"<input type=\\\"hidden\\\" name=\\\"username\\\" value=\\\"$user\\\">\";\n");
					fprintf(file, "                echo '<input type=\"submit\" value=\"Re-login / Add or Remove Author\">';\n");
					fprintf(file, "                echo '</form>';\n");
					fprintf(file, "            }\n");
					fprintf(file, "            else if ($output[$i] == \"-1\")\n");
                                        fprintf(file, "            {\n");
                                        fprintf(file, "                echo \"<hr>Empty user field, click below to try again<hr><br>\";\n");
                                        fprintf(file, "                echo '<form action=\"index.php\" method=\"POST\">';\n");
                                        fprintf(file, "                echo \"<input type=\\\"hidden\\\" name=\\\"username\\\" value=\\\"$user\\\">\";\n");
                                        fprintf(file, "                echo '<input type=\"submit\" value=\"Re-login\">';\n");
                                        fprintf(file, "                echo '</form>';\n");
                                        fprintf(file, "            }\n");
					fprintf(file, "        }\n");
					fprintf(file, "    }\n");
					fprintf(file, "?>\n");
				}
				/*Home tag*/
                		else if (line[i] == '3')
               			{
               			     fprintf(file, "<?php\n");
               			     fprintf(file, "    exec('./parseConfig addauthor.wpml');\n");
                		     fprintf(file, "    exec('./parseConfig streams.wpml');\n");
               			     fprintf(file, "    exec('./parseConfig post.wpml');\n");
                		     fprintf(file, "?>");
                		}
				/*Button*/
				else if (line[i] == 'b')
				{
					char *name, *link, *hidden, *temp;

					name = malloc(sizeof(char));
					link = malloc(sizeof(char));
					hidden = malloc(sizeof(char));
					name[0] = '\0';
					link[0] = '\0';
					hidden[0] = '\0';

					/*Skip past (*/
					i += 2;
					while (line[i] != ')')
					{
						/*get the name of description*/
						temp = getName(line, &i, longestLine);

						if (line[i] == '"') i++;

						if (strcmp(temp, "name") == 0)
						{
							free(name);
							name = getDes(line, &i, longestLine);
						}
						else if (strcmp(temp, "link") == 0)
						{
							free(link);
							link = getDes(line, &i, longestLine);
						}
						else if (strcmp(temp, "hidden") == 0)
						{
							free(hidden);
							hidden = getDes(line, &i, longestLine);
						}

						if (line[i] == ',') i++;

						free(temp);
					}

					/*Default*/
					if (link[0] == '\0')
						fprintf(file, "<form method=\"POST\" action=\"\">\n");
					/*Not default*/
					else if (link[0] != '\0')
						fprintf(file, "<form method=\"POST\" action=\"%s\">\n", link);

					if (hidden[0] != '\0')
						fprintf(file, "<input type=\"hidden\" name=\"username\" value=\"%s\">\n", hidden);

					/*Default*/
					if (name[0] == '\0')
						fprintf(file, "<input type=\"submit\" value=\"\">\n</form>\n\n");
					/*Not default*/
					else if (name[0] != '\0')
						fprintf(file, "<input type=\"submit\" value=\"%s\">\n</form>\n\n", name);

					free(name);
					free(link);
					free(hidden);
				}
				/*Draw horizontal line*/
				else if (line[i] == 'd')
				{
					/*Just print horizontal line*/
					fprintf(file, "<HR>\n\n");
				}
				/*Executable*/
				else if (line[i] == 'e')
				{

					char *exe, *temp;

					exe = malloc(sizeof(char));
					exe[0] = '\0';

					/*Skip past (*/
					i += 2;
					while (line[i] != ')')
					{
						/*get the name of description*/
						temp = getName(line, &i, longestLine);

						if (line[i] == '"') i++;

						if (strcmp(temp, "exe") == 0)
						{
							free(exe);
							exe = getDes(line, &i, longestLine);
						}

						if (line[i] == ',') i++;

						free(temp);
					}

					/*Default*/
					fprintf(file, "<?php\n");
					fprintf(file, "    $c = './%s';\n", exe);
					fprintf(file, "    exec($c, $o, $s);\n");
					fprintf(file, "    if ($s)\n");
					fprintf(file, "        echo \"FAILED EXECUTION OF FILE\";\n");
					fprintf(file, "    else {\n");
					fprintf(file, "        foreach($o as $l)\n");
					fprintf(file, "            echo $l;\n");
					fprintf(file, "    }\n");
					fprintf(file, "?>\n");

					free(exe);
				}
				/*Heading*/
				else if (line[i] == 'h')
				{
					char *text, *size, *temp;

					text = malloc(sizeof(char));
					size = malloc(sizeof(char));
					text[0] = '\0';
					size[0] = '\0';

					/*Skip past (*/
					i += 2;
					while (line[i] != ')')
					{
						/*get the name of description*/
						temp = getName(line, &i, longestLine);

						if (line[i] == '"') i++;

						if (strcmp(temp, "text") == 0)
						{
							free(text);
							text = getDes(line, &i, longestLine);
						}
						else if (strcmp(temp, "size") == 0)
						{
							free(size);
							size = getDes(line, &i, longestLine);
						}

						if (line[i] == ',') i++;

						free(temp);
					}

					/*Default*/
					if (size[0] == '\0')
						fprintf(file, "<h3>");
					/*Not default*/
					else if (size[0] != '\0')
						fprintf(file, "<h%s>", size);

					/*Default*/
					if (text[0] == '\0')
						fprintf(file, "HEADING");
					/*Not default*/
					else if (text[0] != '\0')
						fprintf(file, "%s", text);

					/*Default*/
					if (size[0] == '\0')
						fprintf(file, "</h3>\n\n");
					/*Not default*/
					else if (size[0] != '\0')
						fprintf(file, "</h%s>\n\n", size);

					free(size);
					free(text);
				}
				/*Input*/
				else if (line[i] == 'i')
				{
					int a = 0, t = 0, n = 0, v = 0, printB = 0;
					char *action, *text, *name, *hidden, *field, *value, *temp;

					action = malloc(sizeof(char));
					text = malloc(sizeof(char));
					name = malloc(sizeof(char));
					value = malloc(sizeof(char));
					hidden = malloc(sizeof(char));
					field = malloc(sizeof(char));

					action[0] = '\0';
					text[0] = '\0';
					name[0] = '\0';
					value[0] = '\0';
					hidden[0] = '\0';
					field[0] = '\0';

					/*Skip past (*/
					i += 2;
					while (line[i] != ')')
					{
						/*get the name of description*/
						temp = getName(line, &i, longestLine);

						if (line[i] == '"') i++;

						if (strcmp(temp, "action") == 0)
						{
							free(action);
							action = getDes(line, &i, longestLine);
						}
						else if (strcmp(temp, "text") == 0)
						{
							free(text);
							text = getDes(line, &i, longestLine);
							t = 1;
						}
						else if (strcmp(temp, "name") == 0)
						{
							free(name);
							name = getDes(line, &i, longestLine);
							n = 1;
						}
						else if (strcmp(temp, "value") == 0)
						{
							free(value);
							value = getDes(line, &i, longestLine);
							v = 1;
						}
						else if (strcmp(temp, "hidden") == 0)
                        			{
                            				free(hidden);
                            				hidden = getDes(line, &i, longestLine);
                        			}
						else if (strcmp(temp, "field") == 0)
						{
							free(field);
							field = getDes(line, &i, longestLine);
						}

						if (line[i] == ',') i++;

						if (a == 0)
						{
							fprintf(file, "<form method=\"POST\" action=\"%s\">\n", action);
							a = 1;
						}

						if (t == 1 && n == 1 && v == 1)
						{
							fprintf(file, "%s<input type=\"text\" name=\"%s\" value=\"%s\"><br>\n", text, name, value);

							t = 0;
							n = 0;
							v = 0;
							printB = 1;
						}

						free(temp);
					}

					if (hidden[0] != '\0')
                    			{
                        			fprintf(file, "<input type=\"hidden\" name=\"username\" value=\"%s\">\n", hidden);
                    			}

					if (field[0] != '\0')
                    				fprintf(file, "<textarea name=\"%s\" rows=\"15\" cols=\"60\"></textarea><br>", field);


					if (printB == 1)
						fprintf(file, "<input type=\"submit\" value=\"Submit\"><br>\n");

					if (a == 1)
						fprintf(file, "</form>\n\n");

					free(action);
					free(text);
					free(name);
					free(value);
					free(field);
					free(hidden);
				}
				/*Link*/
				else if (line[i] == 'l')
				{
					char *text, *link, *temp;

					text = malloc(sizeof(char));
					link = malloc(sizeof(char));
					text[0] = '\0';
					link[0] = '\0';

					/*Skip past (*/
					i += 2;
					while (line[i] != ')')
					{
						/*get the name of description*/
						temp = getName(line, &i, longestLine);

						if (line[i] == '"') i++;

						if (strcmp(temp, "text") == 0)
						{
							free(text);
							text = getDes(line, &i, longestLine);
						}
						else if (strcmp(temp, "link") == 0)
						{
							free(link);
							link = getDes(line, &i, longestLine);
						}

						if (line[i] == ',') i++;
						
						free(temp);
					}

					/*Default*/
					if (link[0] == '\0')
						fprintf(file, "<a href=\"\">");
					/*Not default*/
					else if (link[0] != '\0')
						fprintf(file, "<a href=\"%s\">", link);

					/*Default*/
					if (text[0] == '\0')
						fprintf(file, "link</a>\n\n");
					/*Not default*/
					else if (text[0] != '\0')
						fprintf(file, "%s</a>\n\n", text);

					free(text);
					free(link);
				}
				/*Picture*/
				else if (line[i] == 'p')
				{
					char *image, *size, *temp;

					image = malloc(sizeof(char));
					size = malloc(sizeof(char));
					image[0] = '\0';
					size[0] = '\0';

					/*Skip past (*/
					i += 2;
					while (line[i] != ')')
					{
						/*get the name of description*/
						temp = getName(line, &i, longestLine);

						if (line[i] == '"') i++;

						if (strcmp(temp, "image") == 0)
						{
							free(image);
							image = getDes(line, &i, longestLine);
						}
						else if (strcmp(temp, "size") == 0)
						{
							free(size);
							size = getDes(line, &i, longestLine);
						}

						if (line[i] == ',') i++;
						
						free(temp);
					}

					/*Default*/
					if (image[0] == '\0')
						fprintf(file, "<img src=\"\" alt=\"User image\" ");
					/*Not default*/
					else if (image[0] != '\0')
						fprintf(file, "<img src=\"%s\" alt=\"User image\" ", image);

					/*Default*/
					if (size[0] == '\0')
						fprintf(file, "style=\"width:100px;height:100px;\">\n\n");
					/*Not default*/
					else if (size[0] != '\0')
					{
						fprintf(file, "style=\"width:");
						int j = 0;
						while (size[j] != 'x' && size[j] != 'X')
						{
							fprintf(file, "%c", size[j]);
							j++;
						}
						j++;

						fprintf(file, "px;height:");
						while (j < strlen(size))
						{
							fprintf(file, "%c", size[j]);
							j++;
						}
						fprintf(file, "px;\">\n\n");
					}

					free(size);
					free(image);
				}
				/*Radio button*/
				else if (line[i] == 'r')
				{
					int currVal = 0, j, a = 0, n = 0, v = 0, once = 0;
					char *action, *name, *temp, **value;

					action = malloc(sizeof(char));
					name = malloc(sizeof(char));
					action[0] = '\0';
					name[0] = '\0';

					value = malloc(sizeof(char*) * numCom);
					for (j = 0; j < numCom; j++)
						value[j] = malloc(sizeof(char) * longestLine);

					/*Skip past (*/
					i += 2;
					while (line[i] != ')')
					{
						/*get the name of description*/
						temp = getName(line, &i, longestLine);

						if (line[i] == '"') i++;

						if (strcmp(temp, "action") == 0)
						{
							free(action);
							action = getDes(line, &i, longestLine);
							a = 1;
						}
						else if (strcmp(temp, "name") == 0)
						{
							free(name);
							name = getDes(line, &i, longestLine);
							n = 1;
						}
						else if (strcmp(temp, "value") == 0)
						{
							free(value[currVal]);
							value[currVal] = getDes(line, &i, longestLine);
							currVal++;
							v = 1;
						}

						if (line[i] == ',') i++;
						
						free(temp);
					}

					if (a == 1 && n == 1 && v == 1)
					{
						fprintf(file, "<form method=\"POST\" action=\"%s\">\n", action);
						for (j = 0; j < currVal; j++)
						{
							if (once == 0)
							{
								fprintf(file, "<input type=\"radio\" name=\"%s\" value=\"%s\" checked> %s<br>\n", name, value[j], value[j]);
								once = 1;
							}
							else
								fprintf(file, "<input type=\"radio\" name=\"%s\" value=\"%s\"> %s<br>\n", name, value[j], value[j]);
						}
						fprintf(file, "</form>\n\n");
					}

					for (j = 0; j < numCom; j++)
						free(value[j]);
					free(value);
					free(action);
					free(name);
				}
				/*Text*/
				else if (line[i] == 't')
				{
					char *text, *f, *temp;

					text = malloc(sizeof(char));
					f = malloc(sizeof(char));
					text[0] = '\0';
					f[0] = '\0';

					/*Skip past (*/
					i += 2;
					while (line[i] != ')')
					{
						/*get the name of description*/
						temp = getName(line, &i, longestLine);

						if (line[i] == '"') i++;

						if (strcmp(temp, "text") == 0)
						{
							free(text);
							text = getDes(line, &i, longestLine);
						}
						else if (strcmp(temp, "file") == 0)
						{
							free(f);
							f = getDes(line, &i, longestLine);
						}

						if (line[i] == ',') i++;
						
						free(temp);
					}

					int noPrint = 0;
					
					/*Not default*/
					if (f[0] != '\0')
					{
						FILE *filePrint;

						filePrint = fopen(f, "r");

						if (filePrint)
						{
							char ch;
							while ((ch = fgetc(filePrint)) != EOF)
								fprintf(file, "%c", ch);

							fclose(filePrint);
						}
						noPrint = 1;
					}

					if (noPrint == 0)
					{
						/*Not default*/
						if (text[0] != '\0')
						{
							fprintf(file, "%s", text);
							noPrint = 1;
						}
					}

					if (noPrint == 0)
					{
						/*Default*/
						if (f[0] == '\0' || text[0] == '\0')
							fprintf(file, "Default text");
					}

					free(text);
					free(f);
				}
				else if (line[i] == 'n')
				{
					fprintf(file, "<HTML>\n<BODY>\n\n");
				}
				else if (line[i] == 'm')
				{
					fprintf(file, "</BODY>\n</HTML>");
				}
			}
		}
	}

	if (strcmp(nameOfFile, "index.php") != 0)
	{
		fclose(file);
	}
	free(line);
}

char * getName(char *line, int *i, int x)
{
	char *toReturn = malloc(sizeof(char) * x);
	toReturn[0] = '\0';

	while (line[*i] != '=')
	{
		int len = strlen(toReturn);
		toReturn[len] = line[*i];
		toReturn[len + 1] = '\0';
		*i = *i + 1;
	}
	*i = *i + 1;

	return toReturn;
}

char * getDes(char *line, int *i, int x)
{
	char *toReturn = malloc(sizeof(char) * x);
	toReturn[0] = '\0';

	if (line[*i - 1] != '"')
	{
		while (line[*i] != ',' && line[*i] != ')')
		{
			int len = strlen(toReturn);
			toReturn[len] = line[*i];
			toReturn[len + 1] = '\0';
			*i = *i + 1;
		}
	}
	else
	{
		while ((line[*i + 1] != ',') && line[*i + 1] != ')')
		{
			int len = strlen(toReturn);
			toReturn[len] = line[*i];
			toReturn[len + 1] = '\0';
			*i = *i + 1;
		}
		*i = *i + 1;
	}

	return toReturn;
}

void checkArguments(int x, int y)
{
	if (x != y)
	{
		printf("Incorrect number of command line arguments (format: ./parseConfig <filename>)\nparseConfig will now exit.\n");
		exit(0);
	}
}

int longestPossibleLine(FILE *fp)
{
	int toReturn = 1;
	char c;
	while ((c = fgetc(fp)) != EOF) toReturn++;
	rewind(fp);
	return toReturn;
}

int numCommas(FILE *fp)
{
	int toReturn = 1;
	char c;
	while ((c = fgetc(fp)) != EOF)
	{
		if (c == ',')
			toReturn++;
	}
	rewind(fp);
	toReturn = toReturn + 1;
	toReturn = toReturn * 2;
	return toReturn;
}
